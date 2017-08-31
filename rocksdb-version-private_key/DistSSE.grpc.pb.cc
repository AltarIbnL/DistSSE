// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: DistSSE.proto

#include "DistSSE.pb.h"
#include "DistSSE.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>
namespace DistSSE {

static const char* RPC_method_names[] = {
  "/DistSSE.RPC/setup",
  "/DistSSE.RPC/search",
  "/DistSSE.RPC/update",
  "/DistSSE.RPC/batch_update",
  "/DistSSE.RPC/cache",
  "/DistSSE.RPC/batch_cache",
};

std::unique_ptr< RPC::Stub> RPC::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< RPC::Stub> stub(new RPC::Stub(channel));
  return stub;
}

RPC::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_setup_(RPC_method_names[0], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_search_(RPC_method_names[1], ::grpc::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_update_(RPC_method_names[2], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_batch_update_(RPC_method_names[3], ::grpc::RpcMethod::CLIENT_STREAMING, channel)
  , rpcmethod_cache_(RPC_method_names[4], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_batch_cache_(RPC_method_names[5], ::grpc::RpcMethod::CLIENT_STREAMING, channel)
  {}

::grpc::Status RPC::Stub::setup(::grpc::ClientContext* context, const ::DistSSE::SetupRequestMessage& request, ::DistSSE::ExecuteStatus* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_setup_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::DistSSE::ExecuteStatus>* RPC::Stub::AsyncsetupRaw(::grpc::ClientContext* context, const ::DistSSE::SetupRequestMessage& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::DistSSE::ExecuteStatus>(channel_.get(), cq, rpcmethod_setup_, context, request);
}

::grpc::ClientReader< ::DistSSE::SearchReply>* RPC::Stub::searchRaw(::grpc::ClientContext* context, const ::DistSSE::SearchRequestMessage& request) {
  return new ::grpc::ClientReader< ::DistSSE::SearchReply>(channel_.get(), rpcmethod_search_, context, request);
}

::grpc::ClientAsyncReader< ::DistSSE::SearchReply>* RPC::Stub::AsyncsearchRaw(::grpc::ClientContext* context, const ::DistSSE::SearchRequestMessage& request, ::grpc::CompletionQueue* cq, void* tag) {
  return new ::grpc::ClientAsyncReader< ::DistSSE::SearchReply>(channel_.get(), cq, rpcmethod_search_, context, request, tag);
}

::grpc::Status RPC::Stub::update(::grpc::ClientContext* context, const ::DistSSE::UpdateRequestMessage& request, ::DistSSE::ExecuteStatus* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_update_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::DistSSE::ExecuteStatus>* RPC::Stub::AsyncupdateRaw(::grpc::ClientContext* context, const ::DistSSE::UpdateRequestMessage& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::DistSSE::ExecuteStatus>(channel_.get(), cq, rpcmethod_update_, context, request);
}

::grpc::ClientWriter< ::DistSSE::UpdateRequestMessage>* RPC::Stub::batch_updateRaw(::grpc::ClientContext* context, ::DistSSE::ExecuteStatus* response) {
  return new ::grpc::ClientWriter< ::DistSSE::UpdateRequestMessage>(channel_.get(), rpcmethod_batch_update_, context, response);
}

::grpc::ClientAsyncWriter< ::DistSSE::UpdateRequestMessage>* RPC::Stub::Asyncbatch_updateRaw(::grpc::ClientContext* context, ::DistSSE::ExecuteStatus* response, ::grpc::CompletionQueue* cq, void* tag) {
  return new ::grpc::ClientAsyncWriter< ::DistSSE::UpdateRequestMessage>(channel_.get(), cq, rpcmethod_batch_update_, context, response, tag);
}

::grpc::Status RPC::Stub::cache(::grpc::ClientContext* context, const ::DistSSE::CacheRequestMessage& request, ::DistSSE::ExecuteStatus* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_cache_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::DistSSE::ExecuteStatus>* RPC::Stub::AsynccacheRaw(::grpc::ClientContext* context, const ::DistSSE::CacheRequestMessage& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::DistSSE::ExecuteStatus>(channel_.get(), cq, rpcmethod_cache_, context, request);
}

::grpc::ClientWriter< ::DistSSE::CacheRequestMessage>* RPC::Stub::batch_cacheRaw(::grpc::ClientContext* context, ::DistSSE::ExecuteStatus* response) {
  return new ::grpc::ClientWriter< ::DistSSE::CacheRequestMessage>(channel_.get(), rpcmethod_batch_cache_, context, response);
}

::grpc::ClientAsyncWriter< ::DistSSE::CacheRequestMessage>* RPC::Stub::Asyncbatch_cacheRaw(::grpc::ClientContext* context, ::DistSSE::ExecuteStatus* response, ::grpc::CompletionQueue* cq, void* tag) {
  return new ::grpc::ClientAsyncWriter< ::DistSSE::CacheRequestMessage>(channel_.get(), cq, rpcmethod_batch_cache_, context, response, tag);
}

RPC::Service::Service() {
  AddMethod(new ::grpc::RpcServiceMethod(
      RPC_method_names[0],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< RPC::Service, ::DistSSE::SetupRequestMessage, ::DistSSE::ExecuteStatus>(
          std::mem_fn(&RPC::Service::setup), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      RPC_method_names[1],
      ::grpc::RpcMethod::SERVER_STREAMING,
      new ::grpc::ServerStreamingHandler< RPC::Service, ::DistSSE::SearchRequestMessage, ::DistSSE::SearchReply>(
          std::mem_fn(&RPC::Service::search), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      RPC_method_names[2],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< RPC::Service, ::DistSSE::UpdateRequestMessage, ::DistSSE::ExecuteStatus>(
          std::mem_fn(&RPC::Service::update), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      RPC_method_names[3],
      ::grpc::RpcMethod::CLIENT_STREAMING,
      new ::grpc::ClientStreamingHandler< RPC::Service, ::DistSSE::UpdateRequestMessage, ::DistSSE::ExecuteStatus>(
          std::mem_fn(&RPC::Service::batch_update), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      RPC_method_names[4],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< RPC::Service, ::DistSSE::CacheRequestMessage, ::DistSSE::ExecuteStatus>(
          std::mem_fn(&RPC::Service::cache), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      RPC_method_names[5],
      ::grpc::RpcMethod::CLIENT_STREAMING,
      new ::grpc::ClientStreamingHandler< RPC::Service, ::DistSSE::CacheRequestMessage, ::DistSSE::ExecuteStatus>(
          std::mem_fn(&RPC::Service::batch_cache), this)));
}

RPC::Service::~Service() {
}

::grpc::Status RPC::Service::setup(::grpc::ServerContext* context, const ::DistSSE::SetupRequestMessage* request, ::DistSSE::ExecuteStatus* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RPC::Service::search(::grpc::ServerContext* context, const ::DistSSE::SearchRequestMessage* request, ::grpc::ServerWriter< ::DistSSE::SearchReply>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RPC::Service::update(::grpc::ServerContext* context, const ::DistSSE::UpdateRequestMessage* request, ::DistSSE::ExecuteStatus* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RPC::Service::batch_update(::grpc::ServerContext* context, ::grpc::ServerReader< ::DistSSE::UpdateRequestMessage>* reader, ::DistSSE::ExecuteStatus* response) {
  (void) context;
  (void) reader;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RPC::Service::cache(::grpc::ServerContext* context, const ::DistSSE::CacheRequestMessage* request, ::DistSSE::ExecuteStatus* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RPC::Service::batch_cache(::grpc::ServerContext* context, ::grpc::ServerReader< ::DistSSE::CacheRequestMessage>* reader, ::DistSSE::ExecuteStatus* response) {
  (void) context;
  (void) reader;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace DistSSE
