/*********************************************************************************
 * This code includes contributions from:
 * Jonathas Barbosa (https://github.com/jnthas/improv-wifi-demo)
 * Improv Wi-Fi (https://github.com/improv-wifi/sdk-cpp)
 ********************************************************************************/
#include "Improv.h"
#include "HomeSpan.h"
#include <set>

using namespace Utils;
using namespace improv;

#define MAX_ATTEMPTS_WIFI_CONNECTION 10

//////////////////////////////////////////////////////
//       PROCESS IMPROV COMMANDS FROM SERIAL        //  

void Span::processImprovCommand(const char *c, Span* span){
  
  uint8_t len = c[8] + 10;
  // Copy the const char *c from the argument into a new character array
  char buffer[len];
  strcpy(buffer, c);
  buffer[len-1] = c[len-1]; // Copy the checksum bit since it falls after the null terminator

  improv::parseImprovSerialByte(len - 1, buffer[len - 1], (uint8_t *)c, [&](ImprovCommand command) {
    improv::handleImprovCommand(command, span);
    return true;
  }, [&](Error error) {
    LOG0("Error parsing Improv command");
  });
} // Span::processImprovCommand

//////////////////////////////////////////////////////
//             IMPROV-SPECIFIC FUNCTIONS            //  

namespace improv {

void handleImprovCommand(improv::ImprovCommand cmd, Span* span) {
  switch(cmd.command) {

    // Configure WiFi credentials
    case Command::WIFI_SETTINGS:
      Serial.println("WiFi Settings: ");
      Serial.print(cmd.ssid.c_str());
      Serial.print(" ");
      Serial.println(cmd.password.c_str());

      // Attempt to use our credentials so we can see if they work
      if (connectWifi(cmd.ssid.c_str(), cmd.password.c_str())) {
        sendImprovState(improv::State::STATE_PROVISIONED);
        std::vector<std::string> infos; // Empty vector, we could put an HTTP URL here as the next step for the user if we had one
                                        // IDEA: We could link to a page that will render the QR code for the user to scan
        std::vector<uint8_t> data = improv::buildRPCResponse(improv::WIFI_SETTINGS, infos, false);
        improv::sendImprovResponse(data);

        span->setWifiCredentials(cmd.ssid.c_str(), cmd.password.c_str()); // Save credentials and reboot
        delay(1000); // Give the serial on the other end a moment to process
        span->reboot();
      } else {
        sendImprovState(improv::State::STATE_STOPPED);
        improv::sendImprovError(improv::Error::ERROR_UNABLE_TO_CONNECT);
      }
      break;

    // Notify the client of the current WiFi provisioning stte
    case Command::GET_CURRENT_STATE:
      if((WiFi.status() == WL_CONNECTED)) {
        sendImprovState(improv::State::STATE_PROVISIONED);
        std::vector<std::string> infos; // Empty vector, we could put an HTTP URL here as the next step for the user if we had one
        std::vector<uint8_t> data = improv::buildRPCResponse(improv::GET_CURRENT_STATE, infos, false);
        improv::sendImprovResponse(data);
      } else {
        sendImprovState(improv::State::STATE_AUTHORIZED);
      }
      break;
    
    // Get general information about the device to show in the flash dialog
    case Command::GET_DEVICE_INFO:
      {
        Serial.println("Get Device Info");
        std::vector<std::string> infos = {
          // Firmware name
          "HomeSpan",
          // Firmware version
          HOMESPAN_VERSION,
          // Hardware chip/variant
          "ESP32",
          // Device name
          span->getDisplayName(),
        };
        std::vector<uint8_t> data = improv::buildRPCResponse(improv::GET_DEVICE_INFO, infos, false);
        improv::sendImprovResponse(data);
        break;
      }
    
    // Get a list of WiFi networks to show in the connect dialog
    case Command::GET_WIFI_NETWORKS:
      getAvailableWifiNetworks();
      break;

    case Command::BAD_CHECKSUM:
      //LOG2("Improv-Serial Error: Bad Checksum");
      break;

    case Command::UNKNOWN:
      LOG0("Improv-Serial Error: Unknown command");
      break;
  }
}

///////////////////////////////

bool connectWifi(const char *ssid, const char *pwd) {
  uint8_t attempts = 0;

  //LOG2("Attempting to connect to WiFi SSID %s\n", ssid);
  WiFi.begin(ssid, pwd); // Connect to the network

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); // Wait for the connection to be established
    //LOG2("Attempting to connect to WiFi SSID %s, attempt #%s\n", ssid, String(attempts));

    if (attempts > MAX_ATTEMPTS_WIFI_CONNECTION) { // Give up after a certain number of attempts
      LOG0("Failed to connect to WiFi");
      WiFi.disconnect();
      return false;
    }
    attempts++;
  }

  LOG0("Successfully connected to WiFi SSID %s\n", ssid);
  return true;
} // connectWifi

///////////////////////////////

void getAvailableWifiNetworks() {
  int networkNum = WiFi.scanNetworks();

  std::set<String> ssidSet; // Store unique SSIDs

  // Send one RPC response for each network we found
  for(int id = 0; id < networkNum; ++id) {
    String ssid = WiFi.SSID(id);
    if (ssidSet.count(ssid) == 0) { // Check if SSID is already sent
      std::vector<uint8_t> data = improv::buildRPCResponse(
              improv::GET_WIFI_NETWORKS, {ssid, String(WiFi.RSSI(id)), (WiFi.encryptionType(id) == WIFI_AUTH_OPEN ? "NO" : "YES")}, false);
      improv::sendImprovResponse(data);
      ssidSet.insert(ssid); // Add SSID to the set
      delay(1);
    }
  }
  // Send a blank response to signal the end of the list
  std::vector<uint8_t> data =
          improv::buildRPCResponse(improv::GET_WIFI_NETWORKS, std::vector<std::string>{}, false);
  improv::sendImprovResponse(data);
}

///////////////////////////////

void sendImprovState(improv::State state) {
  std::vector<uint8_t> data = {'I', 'M', 'P', 'R', 'O', 'V'};
  data.resize(11);
  data[6] = improv::IMPROV_SERIAL_VERSION;
  data[7] = improv::TYPE_CURRENT_STATE;
  data[8] = 1;
  data[9] = state;

  uint8_t checksum = 0x00;
  for(uint8_t d : data)
    checksum += d;
  data[10] = checksum;

  Serial.write(data.data(), data.size()); // Send the output
  
  String hexData;
  for (size_t i = 0; i < data.size(); i++) {
    char hex[3];
    sprintf(hex, "%02X", data[i]);
    hexData += hex;
  }
  //LOG2("Wrote Improv Serial state: %s\n", hexData);
} // sendImprovState

///////////////////////////////

void sendImprovError(improv::Error error) {
  std::vector<uint8_t> data = {'I', 'M', 'P', 'R', 'O', 'V'};
  data.resize(11);
  data[6] = improv::IMPROV_SERIAL_VERSION;
  data[7] = improv::TYPE_ERROR_STATE;
  data[8] = 1;
  data[9] = error;

  uint8_t checksum = 0x00;
  for (uint8_t d : data)
    checksum += d;
  data[10] = checksum;

  Serial.write(data.data(), data.size()); // Send the output

  String hexData;
  for (size_t i = 0; i < data.size(); i++) {
    char hex[3];
    sprintf(hex, "%02X", data[i]);
    hexData += hex;
  }
  //LOG2("Wrote Improv Serial error: %s\n", hexData);
}

///////////////////////////////

void sendImprovResponse(std::vector<uint8_t> &response) {
  std::vector<uint8_t> data = {'I', 'M', 'P', 'R', 'O', 'V'};
  data.resize(9);
  data[6] = improv::IMPROV_SERIAL_VERSION;
  data[7] = improv::TYPE_RPC_RESPONSE;
  data[8] = response.size();
  data.insert(data.end(), response.begin(), response.end());

  uint8_t checksum = 0x00;
  for(uint8_t d : data)
    checksum += d;
  data.push_back(checksum);

  Serial.write(data.data(), data.size()); // Send the output

  String hexData;
  for (size_t i = 0; i < data.size(); i++) {
    char hex[3];
    sprintf(hex, "%02X", data[i]);
    hexData += hex;
  }
  //LOG2("Wrote Improv Serial response: %s\n", hexData);
}

///////////////////////////////

// From https://github.com/improv-wifi/sdk-cpp/blob/main/src/improv.cpp
ImprovCommand parseImprovData(const std::vector<uint8_t> &data, bool check_checksum) {
  return parseImprovData(data.data(), data.size(), check_checksum);
} // parseImprovData

///////////////////////////////

ImprovCommand parseImprovData(const uint8_t *data, size_t length, bool check_checksum) {
  ImprovCommand improv_command;
  Command command = (Command) data[0];
  uint8_t data_length = data[1];

  if(data_length != length - 2 - check_checksum) {
    improv_command.command = UNKNOWN;
    return improv_command;
  }

  if(check_checksum) {
    uint8_t checksum = data[length - 1];

    uint32_t calculated_checksum = 0;
    for(uint8_t i = 0; i < length - 1; i++) {
      calculated_checksum += data[i];
    }

    if((uint8_t) calculated_checksum != checksum) {
      improv_command.command = BAD_CHECKSUM;
      return improv_command;
    }
  }

  if(command == WIFI_SETTINGS) {
    uint8_t ssid_length = data[2];
    uint8_t ssid_start = 3;
    size_t ssid_end = ssid_start + ssid_length;

    uint8_t pass_length = data[ssid_end];
    size_t pass_start = ssid_end + 1;
    size_t pass_end = pass_start + pass_length;

    std::string ssid(data + ssid_start, data + ssid_end);
    std::string password(data + pass_start, data + pass_end);
    return {.command = command, .ssid = ssid, .password = password};
  }

  improv_command.command = command;
  return improv_command;
} // parseImprovData

///////////////////////////////

bool parseImprovSerialByte(size_t position, uint8_t byte, const uint8_t *buffer,
                              std::function<bool(ImprovCommand)> &&callback, std::function<void(Error)> &&on_error) {
  if(position == 0)
    return byte == 'I';
  if(position == 1)
    return byte == 'M';
  if(position == 2)
    return byte == 'P';
  if(position == 3)
    return byte == 'R';
  if(position == 4)
    return byte == 'O';
  if(position == 5)
    return byte == 'V';

  if(position == 6)
    return byte == IMPROV_SERIAL_VERSION;

  if(position <= 8)
    return true;

  uint8_t type = buffer[7];
  uint8_t data_len = buffer[8];

  if(position <= 8 + data_len)
    return true;

  if(position == 8 + data_len + 1) {
    uint8_t checksum = 0x00;
    for(size_t i = 0; i < position; i++)
      checksum += buffer[i];

    if(checksum != byte) {
      //LOG2.println("Improv-Serial packet checksum, calculated: " + String(checksum) + ", expecting: " + String(byte));
      on_error(ERROR_INVALID_RPC);
      return false;
    }

    if(type == TYPE_RPC) {
      auto command = parseImprovData(&buffer[9], data_len, false);
      return callback(command);
    }
  }

  return false;
} // parseImprovSerialByte

///////////////////////////////

std::vector<uint8_t> buildRPCResponse(Command command, const std::vector<std::string> &datum, bool add_checksum) {
  std::vector<uint8_t> out;
  uint32_t length = 0;
  out.push_back(command);
  for(const auto &str : datum) {
    uint8_t len = str.length();
    length += len + 1;
    out.push_back(len);
    out.insert(out.end(), str.begin(), str.end());
  }
  out.insert(out.begin() + 1, length);

  if(add_checksum) {
    uint32_t calculated_checksum = 0;

    for(uint8_t byte : out) {
      calculated_checksum += byte;
    }
    out.push_back(calculated_checksum);
  }
  return out;
} // buildRPCResponse

///////////////////////////////

#ifdef ARDUINO
std::vector<uint8_t> buildRPCResponse(Command command, const std::vector<String> &datum, bool add_checksum) {
  std::vector<uint8_t> out;
  uint32_t length = 0;
  out.push_back(command);
  for(const auto &str : datum) {
    uint8_t len = str.length();
    length += len;
    out.push_back(len);
    out.insert(out.end(), str.begin(), str.end());
  }
  out.insert(out.begin() + 1, length);

  if(add_checksum) {
    uint32_t calculated_checksum = 0;

    for(uint8_t byte : out) {
      calculated_checksum += byte;
    }
    out.push_back(calculated_checksum);
  }
  return out;
} // buildRPCResponse
#endif  // ARDUINO

}  // namespace improv