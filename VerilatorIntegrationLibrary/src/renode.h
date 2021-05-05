//
// Copyright (c) 2010-2021 Antmicro
//
// This file is licensed under the MIT License.
// Full license text is available in 'licenses/MIT.txt'.
//
#ifndef Renode_H
#define Renode_H
#include <string>
#include <vector>
#include "buses/bus.h"
#include "../libs/socket-cpp/Socket/TCPClient.h"

// Protocol must be in sync with Renode's ProtocolMessage
#pragma pack(push, 1)
struct Protocol
{
  Protocol(int actionId, uint64_t addr, uint64_t value)
  {
    this->actionId = actionId;
    this->addr = addr;
    this->value = value;
  }

  int actionId;
  uint64_t addr;
  uint64_t value;
};
#pragma pack(pop)

// Action must be in sync with Renode's ActionType
enum Action
{
  invalidAction = 0,
  tickClock = 1,
  writeRequest = 2,
  readRequest = 3,
  resetPeripheral = 4,
  logMessage = 5,
  interrupt = 6,
  disconnect = 7,
  error = 8,
  ok = 9,
  handshake = 10,
  pushData = 11,
  getData = 12
};

class RenodeAgent
{
public:
  RenodeAgent(BaseBus* bus);
  void simulate(int receiverPort, int senderPort);
  void log(int logLevel, const char* data);
  void addBus(BaseBus* bus);
  virtual void pushToAgent(uint64_t addr, uint64_t value);
  virtual uint64_t requestFromAgent(uint64_t addr);
protected:
  virtual void tick(bool countEnable, uint64_t steps);
  virtual void reset();
  virtual void writeToBus(uint64_t addr, uint64_t value);
  virtual void readFromBus(uint64_t addr);
  void mainSocketSend(Protocol message);
  void senderSocketSend(Protocol request);
  void senderSocketSend(const char* text);
  virtual void handleCustomRequestType(Protocol* message);
  std::vector<BaseBus*> interfaces;

private:
  std::unique_ptr<CTCPClient> mainSocket;
  std::unique_ptr<CTCPClient> senderSocket;
  bool isConnected;
  void handshakeValid();
  struct Protocol* receive();
};
#endif
