#include "improv.h"
#include "HomeSpan.h"

using namespace Utils;
using namespace improv;

void Span::processImprovCommand(const char *c){
  
  uint8_t len = c[8] + 10;
  // Copy the const char *c from the argument into a new character array
  char buffer[len];
  strcpy(buffer, c);
  buffer[len-1] = c[len-1]; // Copy the checksum bit since it falls after the null terminator

  Serial.print("Command: ");
  for (size_t i = 0; i < len; i++) {
    Serial.print("0x");
    Serial.print(c[i] < 16 ? "0" : "");
    Serial.print(c[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.println("Processing Improv Command " + String(buffer) + " length " + strlen(buffer));
  Serial.print("Command: ");
  for (size_t i = 0; i < len; i++) {
    Serial.print("0x");
    Serial.print(buffer[i] < 16 ? "0" : "");
    Serial.print(buffer[i], HEX);
    Serial.print(" ");
  }

  Serial.println();
  Serial.println("Length " + String(len) + " char ");
  Serial.println(buffer[len - 1], HEX);
  Serial.println("Onwards...");

  improv::parse_improv_serial_byte(len - 1, buffer[len - 1], (uint8_t *)c, [&](ImprovCommand command) {
    improv::handleImprovCommand(command);
    return true;
  }, [&](Error error) {
    Serial.println("Error parsing Improv command");
  });
} // Span::processImprovCommand


namespace improv {

void handleImprovCommand(improv::ImprovCommand cmd) {
  switch (cmd.command) {
    case Command::WIFI_SETTINGS:
      Serial.println("WiFi Settings: ");
      Serial.print(cmd.ssid.c_str());
      Serial.print(" ");
      Serial.println(cmd.password.c_str());
      //Span::setWifiCredentials(cmd.ssid.c_str(), cmd.password.c_str());
      break;
    case Command::GET_CURRENT_STATE:
      if ((WiFi.status() == WL_CONNECTED)) {
        sendImprovState(improv::State::STATE_PROVISIONED);
        //std::vector<uint8_t> data = improv::build_rpc_response(improv::GET_CURRENT_STATE, getLocalUrl(), false);
        //send_response(data);

      } else {
        sendImprovState(improv::State::STATE_AUTHORIZED);
      }
      break;
    case Command::GET_DEVICE_INFO:
      {
        Serial.println("Get Device Info");
        std::vector<std::string> infos = {
          // Firmware name
          "HomeSpan",
          // Firmware version
          "1.0.0",
          // Hardware chip/variant
          "ESP32",
          // Device name
          "HomeSpanDevice"
        };
        std::vector<uint8_t> data = improv::build_rpc_response(improv::GET_DEVICE_INFO, infos, false);
        improv::sendImprovResponse(data);
        break;
      }
    case Command::GET_WIFI_NETWORKS:
      getAvailableWifiNetworks();
      break;
    case Command::BAD_CHECKSUM:
      Serial.println("Bad Checksum");
      break;
    case Command::UNKNOWN:
      Serial.println("Unknown");
      break;
  }
}

void getAvailableWifiNetworks() {
  int networkNum = WiFi.scanNetworks();

  for (int id = 0; id < networkNum; ++id) { 
    std::vector<uint8_t> data = improv::build_rpc_response(
            improv::GET_WIFI_NETWORKS, {WiFi.SSID(id), String(WiFi.RSSI(id)), (WiFi.encryptionType(id) == WIFI_AUTH_OPEN ? "NO" : "YES")}, false);
    improv::sendImprovResponse(data);
    delay(1);
  }
  //final response
  std::vector<uint8_t> data =
          improv::build_rpc_response(improv::GET_WIFI_NETWORKS, std::vector<std::string>{}, false);
  improv::sendImprovResponse(data);
}

void sendImprovState(improv::State state) {
  std::vector<uint8_t> data = {'I', 'M', 'P', 'R', 'O', 'V'};
  data.resize(11);
  data[6] = improv::IMPROV_SERIAL_VERSION;
  data[7] = improv::TYPE_CURRENT_STATE;
  data[8] = 1;
  data[9] = state;

  uint8_t checksum = 0x00;
  for (uint8_t d : data)
    checksum += d;
  data[10] = checksum;
  Serial.println("Writing " + String(data.size()) + " bytes to Improv");

  Serial.write(data.data(), data.size());
} // sendImprovState

void sendImprovResponse(std::vector<uint8_t> &response) {
  std::vector<uint8_t> data = {'I', 'M', 'P', 'R', 'O', 'V'};
  data.resize(9);
  data[6] = improv::IMPROV_SERIAL_VERSION;
  data[7] = improv::TYPE_RPC_RESPONSE;
  data[8] = response.size();
  data.insert(data.end(), response.begin(), response.end());

  uint8_t checksum = 0x00;
  for (uint8_t d : data)
    checksum += d;
  data.push_back(checksum);

  Serial.write(data.data(), data.size());

  Serial.println("Wrote ");
  for (size_t i = 0; i < data.size(); i++) {
    Serial.print("0x");
    Serial.print(data[i] < 16 ? "0" : "");
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

// From https://github.com/improv-wifi/sdk-cpp/blob/main/src/improv.cpp
ImprovCommand parse_improv_data(const std::vector<uint8_t> &data, bool check_checksum) {
  return parse_improv_data(data.data(), data.size(), check_checksum);
} // parse_improv_data

ImprovCommand parse_improv_data(const uint8_t *data, size_t length, bool check_checksum) {
  ImprovCommand improv_command;
  Command command = (Command) data[0];
  uint8_t data_length = data[1];

  if (data_length != length - 2 - check_checksum) {
    improv_command.command = UNKNOWN;
    return improv_command;
  }

  if (check_checksum) {
    uint8_t checksum = data[length - 1];

    uint32_t calculated_checksum = 0;
    for (uint8_t i = 0; i < length - 1; i++) {
      calculated_checksum += data[i];
    }

    if ((uint8_t) calculated_checksum != checksum) {
      improv_command.command = BAD_CHECKSUM;
      return improv_command;
    }
  }

  if (command == WIFI_SETTINGS) {
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
} // parse_improv_data

bool parse_improv_serial_byte(size_t position, uint8_t byte, const uint8_t *buffer,
                              std::function<bool(ImprovCommand)> &&callback, std::function<void(Error)> &&on_error) {
  if (position == 0)
    return byte == 'I';
  if (position == 1)
    return byte == 'M';
  if (position == 2)
    return byte == 'P';
  if (position == 3)
    return byte == 'R';
  if (position == 4)
    return byte == 'O';
  if (position == 5)
    return byte == 'V';

  if (position == 6)
    return byte == IMPROV_SERIAL_VERSION;

  if (position <= 8)
    return true;

  uint8_t type = buffer[7];
  uint8_t data_len = buffer[8];

  if (position <= 8 + data_len)
    return true;

  if (position == 8 + data_len + 1) {
    uint8_t checksum = 0x00;
    for (size_t i = 0; i < position; i++)
      checksum += buffer[i];

    Serial.println("Checksum: " + String(checksum) + " Byte: " + String(byte));
    if (checksum != byte) {
      on_error(ERROR_INVALID_RPC);
      return false;
    }

    if (type == TYPE_RPC) {
      auto command = parse_improv_data(&buffer[9], data_len, false);
      return callback(command);
    }
  }

  return false;
} // parse_improv_serial_byte

std::vector<uint8_t> build_rpc_response(Command command, const std::vector<std::string> &datum, bool add_checksum) {
  std::vector<uint8_t> out;
  uint32_t length = 0;
  out.push_back(command);
  for (const auto &str : datum) {
    uint8_t len = str.length();
    length += len + 1;
    out.push_back(len);
    out.insert(out.end(), str.begin(), str.end());
  }
  out.insert(out.begin() + 1, length);

  if (add_checksum) {
    uint32_t calculated_checksum = 0;

    for (uint8_t byte : out) {
      calculated_checksum += byte;
    }
    out.push_back(calculated_checksum);
  }
  return out;
} // build_rpc_response

#ifdef ARDUINO
std::vector<uint8_t> build_rpc_response(Command command, const std::vector<String> &datum, bool add_checksum) {
  std::vector<uint8_t> out;
  uint32_t length = 0;
  out.push_back(command);
  for (const auto &str : datum) {
    uint8_t len = str.length();
    length += len;
    out.push_back(len);
    out.insert(out.end(), str.begin(), str.end());
  }
  out.insert(out.begin() + 1, length);

  if (add_checksum) {
    uint32_t calculated_checksum = 0;

    for (uint8_t byte : out) {
      calculated_checksum += byte;
    }
    out.push_back(calculated_checksum);
  }
  return out;
} // build_rpc_response
#endif  // ARDUINO

}  // namespace improv