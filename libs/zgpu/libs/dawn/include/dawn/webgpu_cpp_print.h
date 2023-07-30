
#ifndef WEBGPU_CPP_PRINT_H_
#define WEBGPU_CPP_PRINT_H_

#include "dawn/webgpu_cpp.h"

#include <iomanip>
#include <ios>
#include <ostream>
#include <type_traits>

namespace wgpu {

  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, AdapterType value) {
      switch (value) {
      case AdapterType::DiscreteGPU:
        o << "AdapterType::DiscreteGPU";
        break;
      case AdapterType::IntegratedGPU:
        o << "AdapterType::IntegratedGPU";
        break;
      case AdapterType::CPU:
        o << "AdapterType::CPU";
        break;
      case AdapterType::Unknown:
        o << "AdapterType::Unknown";
        break;
          default:
            o << "AdapterType::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<AdapterType>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, AddressMode value) {
      switch (value) {
      case AddressMode::Repeat:
        o << "AddressMode::Repeat";
        break;
      case AddressMode::MirrorRepeat:
        o << "AddressMode::MirrorRepeat";
        break;
      case AddressMode::ClampToEdge:
        o << "AddressMode::ClampToEdge";
        break;
          default:
            o << "AddressMode::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<AddressMode>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, AlphaMode value) {
      switch (value) {
      case AlphaMode::Premultiplied:
        o << "AlphaMode::Premultiplied";
        break;
      case AlphaMode::Unpremultiplied:
        o << "AlphaMode::Unpremultiplied";
        break;
      case AlphaMode::Opaque:
        o << "AlphaMode::Opaque";
        break;
          default:
            o << "AlphaMode::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<AlphaMode>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, BackendType value) {
      switch (value) {
      case BackendType::Undefined:
        o << "BackendType::Undefined";
        break;
      case BackendType::Null:
        o << "BackendType::Null";
        break;
      case BackendType::WebGPU:
        o << "BackendType::WebGPU";
        break;
      case BackendType::D3D11:
        o << "BackendType::D3D11";
        break;
      case BackendType::D3D12:
        o << "BackendType::D3D12";
        break;
      case BackendType::Metal:
        o << "BackendType::Metal";
        break;
      case BackendType::Vulkan:
        o << "BackendType::Vulkan";
        break;
      case BackendType::OpenGL:
        o << "BackendType::OpenGL";
        break;
      case BackendType::OpenGLES:
        o << "BackendType::OpenGLES";
        break;
          default:
            o << "BackendType::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<BackendType>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, BlendFactor value) {
      switch (value) {
      case BlendFactor::Zero:
        o << "BlendFactor::Zero";
        break;
      case BlendFactor::One:
        o << "BlendFactor::One";
        break;
      case BlendFactor::Src:
        o << "BlendFactor::Src";
        break;
      case BlendFactor::OneMinusSrc:
        o << "BlendFactor::OneMinusSrc";
        break;
      case BlendFactor::SrcAlpha:
        o << "BlendFactor::SrcAlpha";
        break;
      case BlendFactor::OneMinusSrcAlpha:
        o << "BlendFactor::OneMinusSrcAlpha";
        break;
      case BlendFactor::Dst:
        o << "BlendFactor::Dst";
        break;
      case BlendFactor::OneMinusDst:
        o << "BlendFactor::OneMinusDst";
        break;
      case BlendFactor::DstAlpha:
        o << "BlendFactor::DstAlpha";
        break;
      case BlendFactor::OneMinusDstAlpha:
        o << "BlendFactor::OneMinusDstAlpha";
        break;
      case BlendFactor::SrcAlphaSaturated:
        o << "BlendFactor::SrcAlphaSaturated";
        break;
      case BlendFactor::Constant:
        o << "BlendFactor::Constant";
        break;
      case BlendFactor::OneMinusConstant:
        o << "BlendFactor::OneMinusConstant";
        break;
          default:
            o << "BlendFactor::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<BlendFactor>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, BlendOperation value) {
      switch (value) {
      case BlendOperation::Add:
        o << "BlendOperation::Add";
        break;
      case BlendOperation::Subtract:
        o << "BlendOperation::Subtract";
        break;
      case BlendOperation::ReverseSubtract:
        o << "BlendOperation::ReverseSubtract";
        break;
      case BlendOperation::Min:
        o << "BlendOperation::Min";
        break;
      case BlendOperation::Max:
        o << "BlendOperation::Max";
        break;
          default:
            o << "BlendOperation::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<BlendOperation>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, BufferBindingType value) {
      switch (value) {
      case BufferBindingType::Undefined:
        o << "BufferBindingType::Undefined";
        break;
      case BufferBindingType::Uniform:
        o << "BufferBindingType::Uniform";
        break;
      case BufferBindingType::Storage:
        o << "BufferBindingType::Storage";
        break;
      case BufferBindingType::ReadOnlyStorage:
        o << "BufferBindingType::ReadOnlyStorage";
        break;
          default:
            o << "BufferBindingType::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<BufferBindingType>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, BufferMapAsyncStatus value) {
      switch (value) {
      case BufferMapAsyncStatus::Success:
        o << "BufferMapAsyncStatus::Success";
        break;
      case BufferMapAsyncStatus::ValidationError:
        o << "BufferMapAsyncStatus::ValidationError";
        break;
      case BufferMapAsyncStatus::Unknown:
        o << "BufferMapAsyncStatus::Unknown";
        break;
      case BufferMapAsyncStatus::DeviceLost:
        o << "BufferMapAsyncStatus::DeviceLost";
        break;
      case BufferMapAsyncStatus::DestroyedBeforeCallback:
        o << "BufferMapAsyncStatus::DestroyedBeforeCallback";
        break;
      case BufferMapAsyncStatus::UnmappedBeforeCallback:
        o << "BufferMapAsyncStatus::UnmappedBeforeCallback";
        break;
      case BufferMapAsyncStatus::MappingAlreadyPending:
        o << "BufferMapAsyncStatus::MappingAlreadyPending";
        break;
      case BufferMapAsyncStatus::OffsetOutOfRange:
        o << "BufferMapAsyncStatus::OffsetOutOfRange";
        break;
      case BufferMapAsyncStatus::SizeOutOfRange:
        o << "BufferMapAsyncStatus::SizeOutOfRange";
        break;
          default:
            o << "BufferMapAsyncStatus::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<BufferMapAsyncStatus>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, BufferMapState value) {
      switch (value) {
      case BufferMapState::Unmapped:
        o << "BufferMapState::Unmapped";
        break;
      case BufferMapState::Pending:
        o << "BufferMapState::Pending";
        break;
      case BufferMapState::Mapped:
        o << "BufferMapState::Mapped";
        break;
          default:
            o << "BufferMapState::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<BufferMapState>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, CompareFunction value) {
      switch (value) {
      case CompareFunction::Undefined:
        o << "CompareFunction::Undefined";
        break;
      case CompareFunction::Never:
        o << "CompareFunction::Never";
        break;
      case CompareFunction::Less:
        o << "CompareFunction::Less";
        break;
      case CompareFunction::LessEqual:
        o << "CompareFunction::LessEqual";
        break;
      case CompareFunction::Greater:
        o << "CompareFunction::Greater";
        break;
      case CompareFunction::GreaterEqual:
        o << "CompareFunction::GreaterEqual";
        break;
      case CompareFunction::Equal:
        o << "CompareFunction::Equal";
        break;
      case CompareFunction::NotEqual:
        o << "CompareFunction::NotEqual";
        break;
      case CompareFunction::Always:
        o << "CompareFunction::Always";
        break;
          default:
            o << "CompareFunction::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<CompareFunction>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, CompilationInfoRequestStatus value) {
      switch (value) {
      case CompilationInfoRequestStatus::Success:
        o << "CompilationInfoRequestStatus::Success";
        break;
      case CompilationInfoRequestStatus::Error:
        o << "CompilationInfoRequestStatus::Error";
        break;
      case CompilationInfoRequestStatus::DeviceLost:
        o << "CompilationInfoRequestStatus::DeviceLost";
        break;
      case CompilationInfoRequestStatus::Unknown:
        o << "CompilationInfoRequestStatus::Unknown";
        break;
          default:
            o << "CompilationInfoRequestStatus::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<CompilationInfoRequestStatus>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, CompilationMessageType value) {
      switch (value) {
      case CompilationMessageType::Error:
        o << "CompilationMessageType::Error";
        break;
      case CompilationMessageType::Warning:
        o << "CompilationMessageType::Warning";
        break;
      case CompilationMessageType::Info:
        o << "CompilationMessageType::Info";
        break;
          default:
            o << "CompilationMessageType::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<CompilationMessageType>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, ComputePassTimestampLocation value) {
      switch (value) {
      case ComputePassTimestampLocation::Beginning:
        o << "ComputePassTimestampLocation::Beginning";
        break;
      case ComputePassTimestampLocation::End:
        o << "ComputePassTimestampLocation::End";
        break;
          default:
            o << "ComputePassTimestampLocation::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<ComputePassTimestampLocation>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, CreatePipelineAsyncStatus value) {
      switch (value) {
      case CreatePipelineAsyncStatus::Success:
        o << "CreatePipelineAsyncStatus::Success";
        break;
      case CreatePipelineAsyncStatus::ValidationError:
        o << "CreatePipelineAsyncStatus::ValidationError";
        break;
      case CreatePipelineAsyncStatus::InternalError:
        o << "CreatePipelineAsyncStatus::InternalError";
        break;
      case CreatePipelineAsyncStatus::DeviceLost:
        o << "CreatePipelineAsyncStatus::DeviceLost";
        break;
      case CreatePipelineAsyncStatus::DeviceDestroyed:
        o << "CreatePipelineAsyncStatus::DeviceDestroyed";
        break;
      case CreatePipelineAsyncStatus::Unknown:
        o << "CreatePipelineAsyncStatus::Unknown";
        break;
          default:
            o << "CreatePipelineAsyncStatus::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<CreatePipelineAsyncStatus>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, CullMode value) {
      switch (value) {
      case CullMode::None:
        o << "CullMode::None";
        break;
      case CullMode::Front:
        o << "CullMode::Front";
        break;
      case CullMode::Back:
        o << "CullMode::Back";
        break;
          default:
            o << "CullMode::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<CullMode>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, DeviceLostReason value) {
      switch (value) {
      case DeviceLostReason::Undefined:
        o << "DeviceLostReason::Undefined";
        break;
      case DeviceLostReason::Destroyed:
        o << "DeviceLostReason::Destroyed";
        break;
          default:
            o << "DeviceLostReason::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<DeviceLostReason>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, ErrorFilter value) {
      switch (value) {
      case ErrorFilter::Validation:
        o << "ErrorFilter::Validation";
        break;
      case ErrorFilter::OutOfMemory:
        o << "ErrorFilter::OutOfMemory";
        break;
      case ErrorFilter::Internal:
        o << "ErrorFilter::Internal";
        break;
          default:
            o << "ErrorFilter::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<ErrorFilter>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, ErrorType value) {
      switch (value) {
      case ErrorType::NoError:
        o << "ErrorType::NoError";
        break;
      case ErrorType::Validation:
        o << "ErrorType::Validation";
        break;
      case ErrorType::OutOfMemory:
        o << "ErrorType::OutOfMemory";
        break;
      case ErrorType::Internal:
        o << "ErrorType::Internal";
        break;
      case ErrorType::Unknown:
        o << "ErrorType::Unknown";
        break;
      case ErrorType::DeviceLost:
        o << "ErrorType::DeviceLost";
        break;
          default:
            o << "ErrorType::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<ErrorType>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, ExternalTextureRotation value) {
      switch (value) {
      case ExternalTextureRotation::Rotate0Degrees:
        o << "ExternalTextureRotation::Rotate0Degrees";
        break;
      case ExternalTextureRotation::Rotate90Degrees:
        o << "ExternalTextureRotation::Rotate90Degrees";
        break;
      case ExternalTextureRotation::Rotate180Degrees:
        o << "ExternalTextureRotation::Rotate180Degrees";
        break;
      case ExternalTextureRotation::Rotate270Degrees:
        o << "ExternalTextureRotation::Rotate270Degrees";
        break;
          default:
            o << "ExternalTextureRotation::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<ExternalTextureRotation>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, FeatureName value) {
      switch (value) {
      case FeatureName::Undefined:
        o << "FeatureName::Undefined";
        break;
      case FeatureName::DepthClipControl:
        o << "FeatureName::DepthClipControl";
        break;
      case FeatureName::Depth32FloatStencil8:
        o << "FeatureName::Depth32FloatStencil8";
        break;
      case FeatureName::TimestampQuery:
        o << "FeatureName::TimestampQuery";
        break;
      case FeatureName::PipelineStatisticsQuery:
        o << "FeatureName::PipelineStatisticsQuery";
        break;
      case FeatureName::TextureCompressionBC:
        o << "FeatureName::TextureCompressionBC";
        break;
      case FeatureName::TextureCompressionETC2:
        o << "FeatureName::TextureCompressionETC2";
        break;
      case FeatureName::TextureCompressionASTC:
        o << "FeatureName::TextureCompressionASTC";
        break;
      case FeatureName::IndirectFirstInstance:
        o << "FeatureName::IndirectFirstInstance";
        break;
      case FeatureName::ShaderF16:
        o << "FeatureName::ShaderF16";
        break;
      case FeatureName::RG11B10UfloatRenderable:
        o << "FeatureName::RG11B10UfloatRenderable";
        break;
      case FeatureName::BGRA8UnormStorage:
        o << "FeatureName::BGRA8UnormStorage";
        break;
      case FeatureName::Float32Filterable:
        o << "FeatureName::Float32Filterable";
        break;
      case FeatureName::DawnShaderFloat16:
        o << "FeatureName::DawnShaderFloat16";
        break;
      case FeatureName::DawnInternalUsages:
        o << "FeatureName::DawnInternalUsages";
        break;
      case FeatureName::DawnMultiPlanarFormats:
        o << "FeatureName::DawnMultiPlanarFormats";
        break;
      case FeatureName::DawnNative:
        o << "FeatureName::DawnNative";
        break;
      case FeatureName::ChromiumExperimentalDp4a:
        o << "FeatureName::ChromiumExperimentalDp4a";
        break;
      case FeatureName::TimestampQueryInsidePasses:
        o << "FeatureName::TimestampQueryInsidePasses";
        break;
      case FeatureName::ImplicitDeviceSynchronization:
        o << "FeatureName::ImplicitDeviceSynchronization";
        break;
      case FeatureName::SurfaceCapabilities:
        o << "FeatureName::SurfaceCapabilities";
        break;
      case FeatureName::TransientAttachments:
        o << "FeatureName::TransientAttachments";
        break;
      case FeatureName::MSAARenderToSingleSampled:
        o << "FeatureName::MSAARenderToSingleSampled";
        break;
          default:
            o << "FeatureName::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<FeatureName>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, FilterMode value) {
      switch (value) {
      case FilterMode::Nearest:
        o << "FilterMode::Nearest";
        break;
      case FilterMode::Linear:
        o << "FilterMode::Linear";
        break;
          default:
            o << "FilterMode::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<FilterMode>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, FrontFace value) {
      switch (value) {
      case FrontFace::CCW:
        o << "FrontFace::CCW";
        break;
      case FrontFace::CW:
        o << "FrontFace::CW";
        break;
          default:
            o << "FrontFace::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<FrontFace>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, IndexFormat value) {
      switch (value) {
      case IndexFormat::Undefined:
        o << "IndexFormat::Undefined";
        break;
      case IndexFormat::Uint16:
        o << "IndexFormat::Uint16";
        break;
      case IndexFormat::Uint32:
        o << "IndexFormat::Uint32";
        break;
          default:
            o << "IndexFormat::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<IndexFormat>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, LoadOp value) {
      switch (value) {
      case LoadOp::Undefined:
        o << "LoadOp::Undefined";
        break;
      case LoadOp::Clear:
        o << "LoadOp::Clear";
        break;
      case LoadOp::Load:
        o << "LoadOp::Load";
        break;
          default:
            o << "LoadOp::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<LoadOp>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, LoggingType value) {
      switch (value) {
      case LoggingType::Verbose:
        o << "LoggingType::Verbose";
        break;
      case LoggingType::Info:
        o << "LoggingType::Info";
        break;
      case LoggingType::Warning:
        o << "LoggingType::Warning";
        break;
      case LoggingType::Error:
        o << "LoggingType::Error";
        break;
          default:
            o << "LoggingType::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<LoggingType>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, MipmapFilterMode value) {
      switch (value) {
      case MipmapFilterMode::Nearest:
        o << "MipmapFilterMode::Nearest";
        break;
      case MipmapFilterMode::Linear:
        o << "MipmapFilterMode::Linear";
        break;
          default:
            o << "MipmapFilterMode::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<MipmapFilterMode>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, PipelineStatisticName value) {
      switch (value) {
      case PipelineStatisticName::VertexShaderInvocations:
        o << "PipelineStatisticName::VertexShaderInvocations";
        break;
      case PipelineStatisticName::ClipperInvocations:
        o << "PipelineStatisticName::ClipperInvocations";
        break;
      case PipelineStatisticName::ClipperPrimitivesOut:
        o << "PipelineStatisticName::ClipperPrimitivesOut";
        break;
      case PipelineStatisticName::FragmentShaderInvocations:
        o << "PipelineStatisticName::FragmentShaderInvocations";
        break;
      case PipelineStatisticName::ComputeShaderInvocations:
        o << "PipelineStatisticName::ComputeShaderInvocations";
        break;
          default:
            o << "PipelineStatisticName::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<PipelineStatisticName>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, PowerPreference value) {
      switch (value) {
      case PowerPreference::Undefined:
        o << "PowerPreference::Undefined";
        break;
      case PowerPreference::LowPower:
        o << "PowerPreference::LowPower";
        break;
      case PowerPreference::HighPerformance:
        o << "PowerPreference::HighPerformance";
        break;
          default:
            o << "PowerPreference::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<PowerPreference>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, PresentMode value) {
      switch (value) {
      case PresentMode::Immediate:
        o << "PresentMode::Immediate";
        break;
      case PresentMode::Mailbox:
        o << "PresentMode::Mailbox";
        break;
      case PresentMode::Fifo:
        o << "PresentMode::Fifo";
        break;
          default:
            o << "PresentMode::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<PresentMode>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, PrimitiveTopology value) {
      switch (value) {
      case PrimitiveTopology::PointList:
        o << "PrimitiveTopology::PointList";
        break;
      case PrimitiveTopology::LineList:
        o << "PrimitiveTopology::LineList";
        break;
      case PrimitiveTopology::LineStrip:
        o << "PrimitiveTopology::LineStrip";
        break;
      case PrimitiveTopology::TriangleList:
        o << "PrimitiveTopology::TriangleList";
        break;
      case PrimitiveTopology::TriangleStrip:
        o << "PrimitiveTopology::TriangleStrip";
        break;
          default:
            o << "PrimitiveTopology::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<PrimitiveTopology>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, QueryType value) {
      switch (value) {
      case QueryType::Occlusion:
        o << "QueryType::Occlusion";
        break;
      case QueryType::PipelineStatistics:
        o << "QueryType::PipelineStatistics";
        break;
      case QueryType::Timestamp:
        o << "QueryType::Timestamp";
        break;
          default:
            o << "QueryType::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<QueryType>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, QueueWorkDoneStatus value) {
      switch (value) {
      case QueueWorkDoneStatus::Success:
        o << "QueueWorkDoneStatus::Success";
        break;
      case QueueWorkDoneStatus::Error:
        o << "QueueWorkDoneStatus::Error";
        break;
      case QueueWorkDoneStatus::Unknown:
        o << "QueueWorkDoneStatus::Unknown";
        break;
      case QueueWorkDoneStatus::DeviceLost:
        o << "QueueWorkDoneStatus::DeviceLost";
        break;
          default:
            o << "QueueWorkDoneStatus::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<QueueWorkDoneStatus>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, RenderPassTimestampLocation value) {
      switch (value) {
      case RenderPassTimestampLocation::Beginning:
        o << "RenderPassTimestampLocation::Beginning";
        break;
      case RenderPassTimestampLocation::End:
        o << "RenderPassTimestampLocation::End";
        break;
          default:
            o << "RenderPassTimestampLocation::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<RenderPassTimestampLocation>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, RequestAdapterStatus value) {
      switch (value) {
      case RequestAdapterStatus::Success:
        o << "RequestAdapterStatus::Success";
        break;
      case RequestAdapterStatus::Unavailable:
        o << "RequestAdapterStatus::Unavailable";
        break;
      case RequestAdapterStatus::Error:
        o << "RequestAdapterStatus::Error";
        break;
      case RequestAdapterStatus::Unknown:
        o << "RequestAdapterStatus::Unknown";
        break;
          default:
            o << "RequestAdapterStatus::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<RequestAdapterStatus>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, RequestDeviceStatus value) {
      switch (value) {
      case RequestDeviceStatus::Success:
        o << "RequestDeviceStatus::Success";
        break;
      case RequestDeviceStatus::Error:
        o << "RequestDeviceStatus::Error";
        break;
      case RequestDeviceStatus::Unknown:
        o << "RequestDeviceStatus::Unknown";
        break;
          default:
            o << "RequestDeviceStatus::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<RequestDeviceStatus>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, SType value) {
      switch (value) {
      case SType::Invalid:
        o << "SType::Invalid";
        break;
      case SType::SurfaceDescriptorFromMetalLayer:
        o << "SType::SurfaceDescriptorFromMetalLayer";
        break;
      case SType::SurfaceDescriptorFromWindowsHWND:
        o << "SType::SurfaceDescriptorFromWindowsHWND";
        break;
      case SType::SurfaceDescriptorFromXlibWindow:
        o << "SType::SurfaceDescriptorFromXlibWindow";
        break;
      case SType::SurfaceDescriptorFromCanvasHTMLSelector:
        o << "SType::SurfaceDescriptorFromCanvasHTMLSelector";
        break;
      case SType::ShaderModuleSPIRVDescriptor:
        o << "SType::ShaderModuleSPIRVDescriptor";
        break;
      case SType::ShaderModuleWGSLDescriptor:
        o << "SType::ShaderModuleWGSLDescriptor";
        break;
      case SType::PrimitiveDepthClipControl:
        o << "SType::PrimitiveDepthClipControl";
        break;
      case SType::SurfaceDescriptorFromWaylandSurface:
        o << "SType::SurfaceDescriptorFromWaylandSurface";
        break;
      case SType::SurfaceDescriptorFromAndroidNativeWindow:
        o << "SType::SurfaceDescriptorFromAndroidNativeWindow";
        break;
      case SType::SurfaceDescriptorFromWindowsCoreWindow:
        o << "SType::SurfaceDescriptorFromWindowsCoreWindow";
        break;
      case SType::ExternalTextureBindingEntry:
        o << "SType::ExternalTextureBindingEntry";
        break;
      case SType::ExternalTextureBindingLayout:
        o << "SType::ExternalTextureBindingLayout";
        break;
      case SType::SurfaceDescriptorFromWindowsSwapChainPanel:
        o << "SType::SurfaceDescriptorFromWindowsSwapChainPanel";
        break;
      case SType::RenderPassDescriptorMaxDrawCount:
        o << "SType::RenderPassDescriptorMaxDrawCount";
        break;
      case SType::DawnTextureInternalUsageDescriptor:
        o << "SType::DawnTextureInternalUsageDescriptor";
        break;
      case SType::DawnEncoderInternalUsageDescriptor:
        o << "SType::DawnEncoderInternalUsageDescriptor";
        break;
      case SType::DawnInstanceDescriptor:
        o << "SType::DawnInstanceDescriptor";
        break;
      case SType::DawnCacheDeviceDescriptor:
        o << "SType::DawnCacheDeviceDescriptor";
        break;
      case SType::DawnAdapterPropertiesPowerPreference:
        o << "SType::DawnAdapterPropertiesPowerPreference";
        break;
      case SType::DawnBufferDescriptorErrorInfoFromWireClient:
        o << "SType::DawnBufferDescriptorErrorInfoFromWireClient";
        break;
      case SType::DawnTogglesDescriptor:
        o << "SType::DawnTogglesDescriptor";
        break;
      case SType::DawnShaderModuleSPIRVOptionsDescriptor:
        o << "SType::DawnShaderModuleSPIRVOptionsDescriptor";
        break;
      case SType::RequestAdapterOptionsLUID:
        o << "SType::RequestAdapterOptionsLUID";
        break;
      case SType::RequestAdapterOptionsGetGLProc:
        o << "SType::RequestAdapterOptionsGetGLProc";
        break;
      case SType::DawnMultisampleStateRenderToSingleSampled:
        o << "SType::DawnMultisampleStateRenderToSingleSampled";
        break;
      case SType::DawnRenderPassColorAttachmentRenderToSingleSampled:
        o << "SType::DawnRenderPassColorAttachmentRenderToSingleSampled";
        break;
          default:
            o << "SType::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<SType>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, SamplerBindingType value) {
      switch (value) {
      case SamplerBindingType::Undefined:
        o << "SamplerBindingType::Undefined";
        break;
      case SamplerBindingType::Filtering:
        o << "SamplerBindingType::Filtering";
        break;
      case SamplerBindingType::NonFiltering:
        o << "SamplerBindingType::NonFiltering";
        break;
      case SamplerBindingType::Comparison:
        o << "SamplerBindingType::Comparison";
        break;
          default:
            o << "SamplerBindingType::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<SamplerBindingType>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, StencilOperation value) {
      switch (value) {
      case StencilOperation::Keep:
        o << "StencilOperation::Keep";
        break;
      case StencilOperation::Zero:
        o << "StencilOperation::Zero";
        break;
      case StencilOperation::Replace:
        o << "StencilOperation::Replace";
        break;
      case StencilOperation::Invert:
        o << "StencilOperation::Invert";
        break;
      case StencilOperation::IncrementClamp:
        o << "StencilOperation::IncrementClamp";
        break;
      case StencilOperation::DecrementClamp:
        o << "StencilOperation::DecrementClamp";
        break;
      case StencilOperation::IncrementWrap:
        o << "StencilOperation::IncrementWrap";
        break;
      case StencilOperation::DecrementWrap:
        o << "StencilOperation::DecrementWrap";
        break;
          default:
            o << "StencilOperation::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<StencilOperation>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, StorageTextureAccess value) {
      switch (value) {
      case StorageTextureAccess::Undefined:
        o << "StorageTextureAccess::Undefined";
        break;
      case StorageTextureAccess::WriteOnly:
        o << "StorageTextureAccess::WriteOnly";
        break;
          default:
            o << "StorageTextureAccess::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<StorageTextureAccess>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, StoreOp value) {
      switch (value) {
      case StoreOp::Undefined:
        o << "StoreOp::Undefined";
        break;
      case StoreOp::Store:
        o << "StoreOp::Store";
        break;
      case StoreOp::Discard:
        o << "StoreOp::Discard";
        break;
          default:
            o << "StoreOp::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<StoreOp>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, TextureAspect value) {
      switch (value) {
      case TextureAspect::All:
        o << "TextureAspect::All";
        break;
      case TextureAspect::StencilOnly:
        o << "TextureAspect::StencilOnly";
        break;
      case TextureAspect::DepthOnly:
        o << "TextureAspect::DepthOnly";
        break;
      case TextureAspect::Plane0Only:
        o << "TextureAspect::Plane0Only";
        break;
      case TextureAspect::Plane1Only:
        o << "TextureAspect::Plane1Only";
        break;
          default:
            o << "TextureAspect::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<TextureAspect>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, TextureDimension value) {
      switch (value) {
      case TextureDimension::e1D:
        o << "TextureDimension::e1D";
        break;
      case TextureDimension::e2D:
        o << "TextureDimension::e2D";
        break;
      case TextureDimension::e3D:
        o << "TextureDimension::e3D";
        break;
          default:
            o << "TextureDimension::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<TextureDimension>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, TextureFormat value) {
      switch (value) {
      case TextureFormat::Undefined:
        o << "TextureFormat::Undefined";
        break;
      case TextureFormat::R8Unorm:
        o << "TextureFormat::R8Unorm";
        break;
      case TextureFormat::R8Snorm:
        o << "TextureFormat::R8Snorm";
        break;
      case TextureFormat::R8Uint:
        o << "TextureFormat::R8Uint";
        break;
      case TextureFormat::R8Sint:
        o << "TextureFormat::R8Sint";
        break;
      case TextureFormat::R16Uint:
        o << "TextureFormat::R16Uint";
        break;
      case TextureFormat::R16Sint:
        o << "TextureFormat::R16Sint";
        break;
      case TextureFormat::R16Float:
        o << "TextureFormat::R16Float";
        break;
      case TextureFormat::RG8Unorm:
        o << "TextureFormat::RG8Unorm";
        break;
      case TextureFormat::RG8Snorm:
        o << "TextureFormat::RG8Snorm";
        break;
      case TextureFormat::RG8Uint:
        o << "TextureFormat::RG8Uint";
        break;
      case TextureFormat::RG8Sint:
        o << "TextureFormat::RG8Sint";
        break;
      case TextureFormat::R32Float:
        o << "TextureFormat::R32Float";
        break;
      case TextureFormat::R32Uint:
        o << "TextureFormat::R32Uint";
        break;
      case TextureFormat::R32Sint:
        o << "TextureFormat::R32Sint";
        break;
      case TextureFormat::RG16Uint:
        o << "TextureFormat::RG16Uint";
        break;
      case TextureFormat::RG16Sint:
        o << "TextureFormat::RG16Sint";
        break;
      case TextureFormat::RG16Float:
        o << "TextureFormat::RG16Float";
        break;
      case TextureFormat::RGBA8Unorm:
        o << "TextureFormat::RGBA8Unorm";
        break;
      case TextureFormat::RGBA8UnormSrgb:
        o << "TextureFormat::RGBA8UnormSrgb";
        break;
      case TextureFormat::RGBA8Snorm:
        o << "TextureFormat::RGBA8Snorm";
        break;
      case TextureFormat::RGBA8Uint:
        o << "TextureFormat::RGBA8Uint";
        break;
      case TextureFormat::RGBA8Sint:
        o << "TextureFormat::RGBA8Sint";
        break;
      case TextureFormat::BGRA8Unorm:
        o << "TextureFormat::BGRA8Unorm";
        break;
      case TextureFormat::BGRA8UnormSrgb:
        o << "TextureFormat::BGRA8UnormSrgb";
        break;
      case TextureFormat::RGB10A2Unorm:
        o << "TextureFormat::RGB10A2Unorm";
        break;
      case TextureFormat::RG11B10Ufloat:
        o << "TextureFormat::RG11B10Ufloat";
        break;
      case TextureFormat::RGB9E5Ufloat:
        o << "TextureFormat::RGB9E5Ufloat";
        break;
      case TextureFormat::RG32Float:
        o << "TextureFormat::RG32Float";
        break;
      case TextureFormat::RG32Uint:
        o << "TextureFormat::RG32Uint";
        break;
      case TextureFormat::RG32Sint:
        o << "TextureFormat::RG32Sint";
        break;
      case TextureFormat::RGBA16Uint:
        o << "TextureFormat::RGBA16Uint";
        break;
      case TextureFormat::RGBA16Sint:
        o << "TextureFormat::RGBA16Sint";
        break;
      case TextureFormat::RGBA16Float:
        o << "TextureFormat::RGBA16Float";
        break;
      case TextureFormat::RGBA32Float:
        o << "TextureFormat::RGBA32Float";
        break;
      case TextureFormat::RGBA32Uint:
        o << "TextureFormat::RGBA32Uint";
        break;
      case TextureFormat::RGBA32Sint:
        o << "TextureFormat::RGBA32Sint";
        break;
      case TextureFormat::Stencil8:
        o << "TextureFormat::Stencil8";
        break;
      case TextureFormat::Depth16Unorm:
        o << "TextureFormat::Depth16Unorm";
        break;
      case TextureFormat::Depth24Plus:
        o << "TextureFormat::Depth24Plus";
        break;
      case TextureFormat::Depth24PlusStencil8:
        o << "TextureFormat::Depth24PlusStencil8";
        break;
      case TextureFormat::Depth32Float:
        o << "TextureFormat::Depth32Float";
        break;
      case TextureFormat::Depth32FloatStencil8:
        o << "TextureFormat::Depth32FloatStencil8";
        break;
      case TextureFormat::BC1RGBAUnorm:
        o << "TextureFormat::BC1RGBAUnorm";
        break;
      case TextureFormat::BC1RGBAUnormSrgb:
        o << "TextureFormat::BC1RGBAUnormSrgb";
        break;
      case TextureFormat::BC2RGBAUnorm:
        o << "TextureFormat::BC2RGBAUnorm";
        break;
      case TextureFormat::BC2RGBAUnormSrgb:
        o << "TextureFormat::BC2RGBAUnormSrgb";
        break;
      case TextureFormat::BC3RGBAUnorm:
        o << "TextureFormat::BC3RGBAUnorm";
        break;
      case TextureFormat::BC3RGBAUnormSrgb:
        o << "TextureFormat::BC3RGBAUnormSrgb";
        break;
      case TextureFormat::BC4RUnorm:
        o << "TextureFormat::BC4RUnorm";
        break;
      case TextureFormat::BC4RSnorm:
        o << "TextureFormat::BC4RSnorm";
        break;
      case TextureFormat::BC5RGUnorm:
        o << "TextureFormat::BC5RGUnorm";
        break;
      case TextureFormat::BC5RGSnorm:
        o << "TextureFormat::BC5RGSnorm";
        break;
      case TextureFormat::BC6HRGBUfloat:
        o << "TextureFormat::BC6HRGBUfloat";
        break;
      case TextureFormat::BC6HRGBFloat:
        o << "TextureFormat::BC6HRGBFloat";
        break;
      case TextureFormat::BC7RGBAUnorm:
        o << "TextureFormat::BC7RGBAUnorm";
        break;
      case TextureFormat::BC7RGBAUnormSrgb:
        o << "TextureFormat::BC7RGBAUnormSrgb";
        break;
      case TextureFormat::ETC2RGB8Unorm:
        o << "TextureFormat::ETC2RGB8Unorm";
        break;
      case TextureFormat::ETC2RGB8UnormSrgb:
        o << "TextureFormat::ETC2RGB8UnormSrgb";
        break;
      case TextureFormat::ETC2RGB8A1Unorm:
        o << "TextureFormat::ETC2RGB8A1Unorm";
        break;
      case TextureFormat::ETC2RGB8A1UnormSrgb:
        o << "TextureFormat::ETC2RGB8A1UnormSrgb";
        break;
      case TextureFormat::ETC2RGBA8Unorm:
        o << "TextureFormat::ETC2RGBA8Unorm";
        break;
      case TextureFormat::ETC2RGBA8UnormSrgb:
        o << "TextureFormat::ETC2RGBA8UnormSrgb";
        break;
      case TextureFormat::EACR11Unorm:
        o << "TextureFormat::EACR11Unorm";
        break;
      case TextureFormat::EACR11Snorm:
        o << "TextureFormat::EACR11Snorm";
        break;
      case TextureFormat::EACRG11Unorm:
        o << "TextureFormat::EACRG11Unorm";
        break;
      case TextureFormat::EACRG11Snorm:
        o << "TextureFormat::EACRG11Snorm";
        break;
      case TextureFormat::ASTC4x4Unorm:
        o << "TextureFormat::ASTC4x4Unorm";
        break;
      case TextureFormat::ASTC4x4UnormSrgb:
        o << "TextureFormat::ASTC4x4UnormSrgb";
        break;
      case TextureFormat::ASTC5x4Unorm:
        o << "TextureFormat::ASTC5x4Unorm";
        break;
      case TextureFormat::ASTC5x4UnormSrgb:
        o << "TextureFormat::ASTC5x4UnormSrgb";
        break;
      case TextureFormat::ASTC5x5Unorm:
        o << "TextureFormat::ASTC5x5Unorm";
        break;
      case TextureFormat::ASTC5x5UnormSrgb:
        o << "TextureFormat::ASTC5x5UnormSrgb";
        break;
      case TextureFormat::ASTC6x5Unorm:
        o << "TextureFormat::ASTC6x5Unorm";
        break;
      case TextureFormat::ASTC6x5UnormSrgb:
        o << "TextureFormat::ASTC6x5UnormSrgb";
        break;
      case TextureFormat::ASTC6x6Unorm:
        o << "TextureFormat::ASTC6x6Unorm";
        break;
      case TextureFormat::ASTC6x6UnormSrgb:
        o << "TextureFormat::ASTC6x6UnormSrgb";
        break;
      case TextureFormat::ASTC8x5Unorm:
        o << "TextureFormat::ASTC8x5Unorm";
        break;
      case TextureFormat::ASTC8x5UnormSrgb:
        o << "TextureFormat::ASTC8x5UnormSrgb";
        break;
      case TextureFormat::ASTC8x6Unorm:
        o << "TextureFormat::ASTC8x6Unorm";
        break;
      case TextureFormat::ASTC8x6UnormSrgb:
        o << "TextureFormat::ASTC8x6UnormSrgb";
        break;
      case TextureFormat::ASTC8x8Unorm:
        o << "TextureFormat::ASTC8x8Unorm";
        break;
      case TextureFormat::ASTC8x8UnormSrgb:
        o << "TextureFormat::ASTC8x8UnormSrgb";
        break;
      case TextureFormat::ASTC10x5Unorm:
        o << "TextureFormat::ASTC10x5Unorm";
        break;
      case TextureFormat::ASTC10x5UnormSrgb:
        o << "TextureFormat::ASTC10x5UnormSrgb";
        break;
      case TextureFormat::ASTC10x6Unorm:
        o << "TextureFormat::ASTC10x6Unorm";
        break;
      case TextureFormat::ASTC10x6UnormSrgb:
        o << "TextureFormat::ASTC10x6UnormSrgb";
        break;
      case TextureFormat::ASTC10x8Unorm:
        o << "TextureFormat::ASTC10x8Unorm";
        break;
      case TextureFormat::ASTC10x8UnormSrgb:
        o << "TextureFormat::ASTC10x8UnormSrgb";
        break;
      case TextureFormat::ASTC10x10Unorm:
        o << "TextureFormat::ASTC10x10Unorm";
        break;
      case TextureFormat::ASTC10x10UnormSrgb:
        o << "TextureFormat::ASTC10x10UnormSrgb";
        break;
      case TextureFormat::ASTC12x10Unorm:
        o << "TextureFormat::ASTC12x10Unorm";
        break;
      case TextureFormat::ASTC12x10UnormSrgb:
        o << "TextureFormat::ASTC12x10UnormSrgb";
        break;
      case TextureFormat::ASTC12x12Unorm:
        o << "TextureFormat::ASTC12x12Unorm";
        break;
      case TextureFormat::ASTC12x12UnormSrgb:
        o << "TextureFormat::ASTC12x12UnormSrgb";
        break;
      case TextureFormat::R8BG8Biplanar420Unorm:
        o << "TextureFormat::R8BG8Biplanar420Unorm";
        break;
          default:
            o << "TextureFormat::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<TextureFormat>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, TextureSampleType value) {
      switch (value) {
      case TextureSampleType::Undefined:
        o << "TextureSampleType::Undefined";
        break;
      case TextureSampleType::Float:
        o << "TextureSampleType::Float";
        break;
      case TextureSampleType::UnfilterableFloat:
        o << "TextureSampleType::UnfilterableFloat";
        break;
      case TextureSampleType::Depth:
        o << "TextureSampleType::Depth";
        break;
      case TextureSampleType::Sint:
        o << "TextureSampleType::Sint";
        break;
      case TextureSampleType::Uint:
        o << "TextureSampleType::Uint";
        break;
          default:
            o << "TextureSampleType::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<TextureSampleType>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, TextureViewDimension value) {
      switch (value) {
      case TextureViewDimension::Undefined:
        o << "TextureViewDimension::Undefined";
        break;
      case TextureViewDimension::e1D:
        o << "TextureViewDimension::e1D";
        break;
      case TextureViewDimension::e2D:
        o << "TextureViewDimension::e2D";
        break;
      case TextureViewDimension::e2DArray:
        o << "TextureViewDimension::e2DArray";
        break;
      case TextureViewDimension::Cube:
        o << "TextureViewDimension::Cube";
        break;
      case TextureViewDimension::CubeArray:
        o << "TextureViewDimension::CubeArray";
        break;
      case TextureViewDimension::e3D:
        o << "TextureViewDimension::e3D";
        break;
          default:
            o << "TextureViewDimension::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<TextureViewDimension>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, VertexFormat value) {
      switch (value) {
      case VertexFormat::Undefined:
        o << "VertexFormat::Undefined";
        break;
      case VertexFormat::Uint8x2:
        o << "VertexFormat::Uint8x2";
        break;
      case VertexFormat::Uint8x4:
        o << "VertexFormat::Uint8x4";
        break;
      case VertexFormat::Sint8x2:
        o << "VertexFormat::Sint8x2";
        break;
      case VertexFormat::Sint8x4:
        o << "VertexFormat::Sint8x4";
        break;
      case VertexFormat::Unorm8x2:
        o << "VertexFormat::Unorm8x2";
        break;
      case VertexFormat::Unorm8x4:
        o << "VertexFormat::Unorm8x4";
        break;
      case VertexFormat::Snorm8x2:
        o << "VertexFormat::Snorm8x2";
        break;
      case VertexFormat::Snorm8x4:
        o << "VertexFormat::Snorm8x4";
        break;
      case VertexFormat::Uint16x2:
        o << "VertexFormat::Uint16x2";
        break;
      case VertexFormat::Uint16x4:
        o << "VertexFormat::Uint16x4";
        break;
      case VertexFormat::Sint16x2:
        o << "VertexFormat::Sint16x2";
        break;
      case VertexFormat::Sint16x4:
        o << "VertexFormat::Sint16x4";
        break;
      case VertexFormat::Unorm16x2:
        o << "VertexFormat::Unorm16x2";
        break;
      case VertexFormat::Unorm16x4:
        o << "VertexFormat::Unorm16x4";
        break;
      case VertexFormat::Snorm16x2:
        o << "VertexFormat::Snorm16x2";
        break;
      case VertexFormat::Snorm16x4:
        o << "VertexFormat::Snorm16x4";
        break;
      case VertexFormat::Float16x2:
        o << "VertexFormat::Float16x2";
        break;
      case VertexFormat::Float16x4:
        o << "VertexFormat::Float16x4";
        break;
      case VertexFormat::Float32:
        o << "VertexFormat::Float32";
        break;
      case VertexFormat::Float32x2:
        o << "VertexFormat::Float32x2";
        break;
      case VertexFormat::Float32x3:
        o << "VertexFormat::Float32x3";
        break;
      case VertexFormat::Float32x4:
        o << "VertexFormat::Float32x4";
        break;
      case VertexFormat::Uint32:
        o << "VertexFormat::Uint32";
        break;
      case VertexFormat::Uint32x2:
        o << "VertexFormat::Uint32x2";
        break;
      case VertexFormat::Uint32x3:
        o << "VertexFormat::Uint32x3";
        break;
      case VertexFormat::Uint32x4:
        o << "VertexFormat::Uint32x4";
        break;
      case VertexFormat::Sint32:
        o << "VertexFormat::Sint32";
        break;
      case VertexFormat::Sint32x2:
        o << "VertexFormat::Sint32x2";
        break;
      case VertexFormat::Sint32x3:
        o << "VertexFormat::Sint32x3";
        break;
      case VertexFormat::Sint32x4:
        o << "VertexFormat::Sint32x4";
        break;
          default:
            o << "VertexFormat::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<VertexFormat>::type>(value);
      }
      return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, VertexStepMode value) {
      switch (value) {
      case VertexStepMode::Vertex:
        o << "VertexStepMode::Vertex";
        break;
      case VertexStepMode::Instance:
        o << "VertexStepMode::Instance";
        break;
      case VertexStepMode::VertexBufferNotUsed:
        o << "VertexStepMode::VertexBufferNotUsed";
        break;
          default:
            o << "VertexStepMode::" << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<VertexStepMode>::type>(value);
      }
      return o;
  }

  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, BufferUsage value) {
    o << "BufferUsage::";
    if (!static_cast<bool>(value)) {
    // 0 is often explicitly declared as None.
    o << "None";
      return o;
    }

    bool moreThanOneBit = !HasZeroOrOneBits(value);
    if (moreThanOneBit) {
      o << "(";
    }

    bool first = true;
  if (value & BufferUsage::MapRead) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "MapRead";
    value &= ~BufferUsage::MapRead;
  }
  if (value & BufferUsage::MapWrite) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "MapWrite";
    value &= ~BufferUsage::MapWrite;
  }
  if (value & BufferUsage::CopySrc) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "CopySrc";
    value &= ~BufferUsage::CopySrc;
  }
  if (value & BufferUsage::CopyDst) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "CopyDst";
    value &= ~BufferUsage::CopyDst;
  }
  if (value & BufferUsage::Index) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "Index";
    value &= ~BufferUsage::Index;
  }
  if (value & BufferUsage::Vertex) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "Vertex";
    value &= ~BufferUsage::Vertex;
  }
  if (value & BufferUsage::Uniform) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "Uniform";
    value &= ~BufferUsage::Uniform;
  }
  if (value & BufferUsage::Storage) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "Storage";
    value &= ~BufferUsage::Storage;
  }
  if (value & BufferUsage::Indirect) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "Indirect";
    value &= ~BufferUsage::Indirect;
  }
  if (value & BufferUsage::QueryResolve) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "QueryResolve";
    value &= ~BufferUsage::QueryResolve;
  }

    if (static_cast<bool>(value)) {
      if (!first) {
        o << "|";
      }
      o << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<BufferUsage>::type>(value);
    }

    if (moreThanOneBit) {
      o << ")";
    }
    return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, ColorWriteMask value) {
    o << "ColorWriteMask::";
    if (!static_cast<bool>(value)) {
    // 0 is often explicitly declared as None.
    o << "None";
      return o;
    }

    bool moreThanOneBit = !HasZeroOrOneBits(value);
    if (moreThanOneBit) {
      o << "(";
    }

    bool first = true;
  if (value & ColorWriteMask::Red) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "Red";
    value &= ~ColorWriteMask::Red;
  }
  if (value & ColorWriteMask::Green) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "Green";
    value &= ~ColorWriteMask::Green;
  }
  if (value & ColorWriteMask::Blue) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "Blue";
    value &= ~ColorWriteMask::Blue;
  }
  if (value & ColorWriteMask::Alpha) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "Alpha";
    value &= ~ColorWriteMask::Alpha;
  }
  if (value & ColorWriteMask::All) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "All";
    value &= ~ColorWriteMask::All;
  }

    if (static_cast<bool>(value)) {
      if (!first) {
        o << "|";
      }
      o << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<ColorWriteMask>::type>(value);
    }

    if (moreThanOneBit) {
      o << ")";
    }
    return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, MapMode value) {
    o << "MapMode::";
    if (!static_cast<bool>(value)) {
    // 0 is often explicitly declared as None.
    o << "None";
      return o;
    }

    bool moreThanOneBit = !HasZeroOrOneBits(value);
    if (moreThanOneBit) {
      o << "(";
    }

    bool first = true;
  if (value & MapMode::Read) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "Read";
    value &= ~MapMode::Read;
  }
  if (value & MapMode::Write) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "Write";
    value &= ~MapMode::Write;
  }

    if (static_cast<bool>(value)) {
      if (!first) {
        o << "|";
      }
      o << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<MapMode>::type>(value);
    }

    if (moreThanOneBit) {
      o << ")";
    }
    return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, ShaderStage value) {
    o << "ShaderStage::";
    if (!static_cast<bool>(value)) {
    // 0 is often explicitly declared as None.
    o << "None";
      return o;
    }

    bool moreThanOneBit = !HasZeroOrOneBits(value);
    if (moreThanOneBit) {
      o << "(";
    }

    bool first = true;
  if (value & ShaderStage::Vertex) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "Vertex";
    value &= ~ShaderStage::Vertex;
  }
  if (value & ShaderStage::Fragment) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "Fragment";
    value &= ~ShaderStage::Fragment;
  }
  if (value & ShaderStage::Compute) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "Compute";
    value &= ~ShaderStage::Compute;
  }

    if (static_cast<bool>(value)) {
      if (!first) {
        o << "|";
      }
      o << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<ShaderStage>::type>(value);
    }

    if (moreThanOneBit) {
      o << ")";
    }
    return o;
  }
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& o, TextureUsage value) {
    o << "TextureUsage::";
    if (!static_cast<bool>(value)) {
    // 0 is often explicitly declared as None.
    o << "None";
      return o;
    }

    bool moreThanOneBit = !HasZeroOrOneBits(value);
    if (moreThanOneBit) {
      o << "(";
    }

    bool first = true;
  if (value & TextureUsage::CopySrc) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "CopySrc";
    value &= ~TextureUsage::CopySrc;
  }
  if (value & TextureUsage::CopyDst) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "CopyDst";
    value &= ~TextureUsage::CopyDst;
  }
  if (value & TextureUsage::TextureBinding) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "TextureBinding";
    value &= ~TextureUsage::TextureBinding;
  }
  if (value & TextureUsage::StorageBinding) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "StorageBinding";
    value &= ~TextureUsage::StorageBinding;
  }
  if (value & TextureUsage::RenderAttachment) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "RenderAttachment";
    value &= ~TextureUsage::RenderAttachment;
  }
  if (value & TextureUsage::TransientAttachment) {
    if (!first) {
      o << "|";
    }
    first = false;
    o << "TransientAttachment";
    value &= ~TextureUsage::TransientAttachment;
  }

    if (static_cast<bool>(value)) {
      if (!first) {
        o << "|";
      }
      o << std::showbase << std::hex << std::setfill('0') << std::setw(4) << static_cast<typename std::underlying_type<TextureUsage>::type>(value);
    }

    if (moreThanOneBit) {
      o << ")";
    }
    return o;
  }

}  // namespace wgpu

#endif // WEBGPU_CPP_PRINT_H_
