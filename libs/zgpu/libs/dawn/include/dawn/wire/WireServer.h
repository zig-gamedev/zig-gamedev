// Copyright 2019 The Dawn Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef INCLUDE_DAWN_WIRE_WIRESERVER_H_
#define INCLUDE_DAWN_WIRE_WIRESERVER_H_

#include <memory>

#include "dawn/wire/Wire.h"

struct DawnProcTable;

namespace dawn::wire {

namespace server {
class Server;
class MemoryTransferService;
}  // namespace server

struct DAWN_WIRE_EXPORT WireServerDescriptor {
    const DawnProcTable* procs;
    CommandSerializer* serializer;
    server::MemoryTransferService* memoryTransferService = nullptr;
};

class DAWN_WIRE_EXPORT WireServer : public CommandHandler {
  public:
    explicit WireServer(const WireServerDescriptor& descriptor);
    ~WireServer() override;

    const volatile char* HandleCommands(const volatile char* commands, size_t size) override;

    bool InjectTexture(WGPUTexture texture,
                       uint32_t id,
                       uint32_t generation,
                       uint32_t deviceId,
                       uint32_t deviceGeneration);
    bool InjectSwapChain(WGPUSwapChain swapchain,
                         uint32_t id,
                         uint32_t generation,
                         uint32_t deviceId,
                         uint32_t deviceGeneration);

    bool InjectDevice(WGPUDevice device, uint32_t id, uint32_t generation);

    bool InjectInstance(WGPUInstance instance, uint32_t id, uint32_t generation);

    // Look up a device by (id, generation) pair. Returns nullptr if the generation
    // has expired or the id is not found.
    // The Wire does not have destroy hooks to allow an embedder to observe when an object
    // has been destroyed, but in Chrome, we need to know the list of live devices so we
    // can call device.Tick() on all of them periodically to ensure progress on asynchronous
    // work is made. Getting this list can be done by tracking the (id, generation) of
    // previously injected devices, and observing if GetDevice(id, generation) returns non-null.
    WGPUDevice GetDevice(uint32_t id, uint32_t generation);

    // Check if a device handle is known by the wire.
    // In Chrome, we need to know the list of live devices so we can call device.Tick() on all of
    // them periodically to ensure progress on asynchronous work is made.
    bool IsDeviceKnown(WGPUDevice device) const;

  private:
    std::unique_ptr<server::Server> mImpl;
};

namespace server {
class DAWN_WIRE_EXPORT MemoryTransferService {
  public:
    MemoryTransferService();
    virtual ~MemoryTransferService();

    class ReadHandle;
    class WriteHandle;

    // Deserialize data to create Read/Write handles. These handles are for the client
    // to Read/Write data.
    virtual bool DeserializeReadHandle(const void* deserializePointer,
                                       size_t deserializeSize,
                                       ReadHandle** readHandle) = 0;
    virtual bool DeserializeWriteHandle(const void* deserializePointer,
                                        size_t deserializeSize,
                                        WriteHandle** writeHandle) = 0;

    class DAWN_WIRE_EXPORT ReadHandle {
      public:
        ReadHandle();
        virtual ~ReadHandle();

        // Return the size of the command serialized if
        // SerializeDataUpdate is called with the same offset/size args
        virtual size_t SizeOfSerializeDataUpdate(size_t offset, size_t size) = 0;

        // Gets called when a MapReadCallback resolves.
        // Serialize the data update for the range (offset, offset + size) into
        // |serializePointer| to the client There could be nothing to be serialized (if
        // using shared memory)
        virtual void SerializeDataUpdate(const void* data,
                                         size_t offset,
                                         size_t size,
                                         void* serializePointer) = 0;

      private:
        ReadHandle(const ReadHandle&) = delete;
        ReadHandle& operator=(const ReadHandle&) = delete;
    };

    class DAWN_WIRE_EXPORT WriteHandle {
      public:
        WriteHandle();
        virtual ~WriteHandle();

        // Set the target for writes from the client. DeserializeFlush should copy data
        // into the target.
        void SetTarget(void* data);
        // Set Staging data length for OOB check
        void SetDataLength(size_t dataLength);

        // This function takes in the serialized result of
        // client::MemoryTransferService::WriteHandle::SerializeDataUpdate.
        // Needs to check potential offset/size OOB and overflow
        virtual bool DeserializeDataUpdate(const void* deserializePointer,
                                           size_t deserializeSize,
                                           size_t offset,
                                           size_t size) = 0;

      protected:
        void* mTargetData = nullptr;
        size_t mDataLength = 0;

      private:
        WriteHandle(const WriteHandle&) = delete;
        WriteHandle& operator=(const WriteHandle&) = delete;
    };

  private:
    MemoryTransferService(const MemoryTransferService&) = delete;
    MemoryTransferService& operator=(const MemoryTransferService&) = delete;
};
}  // namespace server

}  // namespace dawn::wire

#endif  // INCLUDE_DAWN_WIRE_WIRESERVER_H_
