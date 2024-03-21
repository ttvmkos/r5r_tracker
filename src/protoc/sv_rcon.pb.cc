// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: sv_rcon.proto

#include "sv_rcon.pb.h"

#include <algorithm>

#include <thirdparty/protobuf/io/coded_stream.h>
#include <thirdparty/protobuf/extension_set.h>
#include <thirdparty/protobuf/wire_format_lite.h>
#include <thirdparty/protobuf/io/zero_copy_stream_impl_lite.h>
// @@protoc_insertion_point(includes)
#include <thirdparty/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace sv_rcon {
PROTOBUF_CONSTEXPR response::response(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_._has_bits_)*/{}
  , /*decltype(_impl_._cached_size_)*/{}
  , /*decltype(_impl_.responsemsg_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.responseval_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.messageid_)*/0
  , /*decltype(_impl_.messagetype_)*/0
  , /*decltype(_impl_.responsetype_)*/0} {}
struct responseDefaultTypeInternal {
  PROTOBUF_CONSTEXPR responseDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~responseDefaultTypeInternal() {}
  union {
    response _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 responseDefaultTypeInternal _response_default_instance_;
}  // namespace sv_rcon
namespace sv_rcon {
bool response_t_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

static ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<std::string> response_t_strings[2] = {};

static const char response_t_names[] =
  "SERVERDATA_RESPONSE_AUTH"
  "SERVERDATA_RESPONSE_CONSOLE_LOG";

static const ::PROTOBUF_NAMESPACE_ID::internal::EnumEntry response_t_entries[] = {
  { {response_t_names + 0, 24}, 0 },
  { {response_t_names + 24, 31}, 1 },
};

static const int response_t_entries_by_number[] = {
  0, // 0 -> SERVERDATA_RESPONSE_AUTH
  1, // 1 -> SERVERDATA_RESPONSE_CONSOLE_LOG
};

const std::string& response_t_Name(
    response_t value) {
  static const bool dummy =
      ::PROTOBUF_NAMESPACE_ID::internal::InitializeEnumStrings(
          response_t_entries,
          response_t_entries_by_number,
          2, response_t_strings);
  (void) dummy;
  int idx = ::PROTOBUF_NAMESPACE_ID::internal::LookUpEnumName(
      response_t_entries,
      response_t_entries_by_number,
      2, value);
  return idx == -1 ? ::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString() :
                     response_t_strings[idx].get();
}
bool response_t_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, response_t* value) {
  int int_value;
  bool success = ::PROTOBUF_NAMESPACE_ID::internal::LookUpEnumValue(
      response_t_entries, 2, name, &int_value);
  if (success) {
    *value = static_cast<response_t>(int_value);
  }
  return success;
}

// ===================================================================

class response::_Internal {
 public:
  using HasBits = decltype(std::declval<response>()._impl_._has_bits_);
  static void set_has_messageid(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static void set_has_messagetype(HasBits* has_bits) {
    (*has_bits)[0] |= 8u;
  }
  static void set_has_responsetype(HasBits* has_bits) {
    (*has_bits)[0] |= 16u;
  }
  static void set_has_responsemsg(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_responseval(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
};

response::response(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::MessageLite(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:sv_rcon.response)
}
response::response(const response& from)
  : ::PROTOBUF_NAMESPACE_ID::MessageLite() {
  response* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){from._impl_._has_bits_}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.responsemsg_){}
    , decltype(_impl_.responseval_){}
    , decltype(_impl_.messageid_){}
    , decltype(_impl_.messagetype_){}
    , decltype(_impl_.responsetype_){}};

  _internal_metadata_.MergeFrom<std::string>(from._internal_metadata_);
  _impl_.responsemsg_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.responsemsg_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (from._internal_has_responsemsg()) {
    _this->_impl_.responsemsg_.Set(from._internal_responsemsg(), 
      _this->GetArenaForAllocation());
  }
  _impl_.responseval_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.responseval_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (from._internal_has_responseval()) {
    _this->_impl_.responseval_.Set(from._internal_responseval(), 
      _this->GetArenaForAllocation());
  }
  ::memcpy(&_impl_.messageid_, &from._impl_.messageid_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.responsetype_) -
    reinterpret_cast<char*>(&_impl_.messageid_)) + sizeof(_impl_.responsetype_));
  // @@protoc_insertion_point(copy_constructor:sv_rcon.response)
}

inline void response::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.responsemsg_){}
    , decltype(_impl_.responseval_){}
    , decltype(_impl_.messageid_){0}
    , decltype(_impl_.messagetype_){0}
    , decltype(_impl_.responsetype_){0}
  };
  _impl_.responsemsg_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.responsemsg_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.responseval_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.responseval_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

response::~response() {
  // @@protoc_insertion_point(destructor:sv_rcon.response)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<std::string>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void response::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.responsemsg_.Destroy();
  _impl_.responseval_.Destroy();
}

void response::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void response::Clear() {
// @@protoc_insertion_point(message_clear_start:sv_rcon.response)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      _impl_.responsemsg_.ClearNonDefaultToEmpty();
    }
    if (cached_has_bits & 0x00000002u) {
      _impl_.responseval_.ClearNonDefaultToEmpty();
    }
  }
  if (cached_has_bits & 0x0000001cu) {
    ::memset(&_impl_.messageid_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&_impl_.responsetype_) -
        reinterpret_cast<char*>(&_impl_.messageid_)) + sizeof(_impl_.responsetype_));
  }
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<std::string>();
}

const char* response::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // optional int32 messageID = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _Internal::set_has_messageid(&has_bits);
          _impl_.messageid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // optional int32 messageType = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          _Internal::set_has_messagetype(&has_bits);
          _impl_.messagetype_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // optional .sv_rcon.response_t responseType = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 24)) {
          uint64_t val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_responsetype(static_cast<::sv_rcon::response_t>(val));
        } else
          goto handle_unusual;
        continue;
      // optional string responseMsg = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 34)) {
          auto str = _internal_mutable_responsemsg();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, nullptr));
        } else
          goto handle_unusual;
        continue;
      // optional string responseVal = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 42)) {
          auto str = _internal_mutable_responseval();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, nullptr));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<std::string>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  _impl_._has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* response::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:sv_rcon.response)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // optional int32 messageID = 1;
  if (_internal_has_messageid()) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(1, this->_internal_messageid(), target);
  }

  // optional int32 messageType = 2;
  if (_internal_has_messagetype()) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(2, this->_internal_messagetype(), target);
  }

  // optional .sv_rcon.response_t responseType = 3;
  if (_internal_has_responsetype()) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteEnumToArray(
      3, this->_internal_responsetype(), target);
  }

  // optional string responseMsg = 4;
  if (_internal_has_responsemsg()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_responsemsg().data(), static_cast<int>(this->_internal_responsemsg().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "sv_rcon.response.responseMsg");
    target = stream->WriteStringMaybeAliased(
        4, this->_internal_responsemsg(), target);
  }

  // optional string responseVal = 5;
  if (_internal_has_responseval()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_responseval().data(), static_cast<int>(this->_internal_responseval().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "sv_rcon.response.responseVal");
    target = stream->WriteStringMaybeAliased(
        5, this->_internal_responseval(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = stream->WriteRaw(_internal_metadata_.unknown_fields<std::string>(::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString).data(),
        static_cast<int>(_internal_metadata_.unknown_fields<std::string>(::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString).size()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:sv_rcon.response)
  return target;
}

size_t response::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:sv_rcon.response)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x0000001fu) {
    // optional string responseMsg = 4;
    if (cached_has_bits & 0x00000001u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_responsemsg());
    }

    // optional string responseVal = 5;
    if (cached_has_bits & 0x00000002u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_responseval());
    }

    // optional int32 messageID = 1;
    if (cached_has_bits & 0x00000004u) {
      total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_messageid());
    }

    // optional int32 messageType = 2;
    if (cached_has_bits & 0x00000008u) {
      total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_messagetype());
    }

    // optional .sv_rcon.response_t responseType = 3;
    if (cached_has_bits & 0x00000010u) {
      total_size += 1 +
        ::_pbi::WireFormatLite::EnumSize(this->_internal_responsetype());
    }

  }
  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    total_size += _internal_metadata_.unknown_fields<std::string>(::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString).size();
  }
  int cached_size = ::_pbi::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void response::CheckTypeAndMergeFrom(
    const ::PROTOBUF_NAMESPACE_ID::MessageLite& from) {
  MergeFrom(*::_pbi::DownCast<const response*>(
      &from));
}

void response::MergeFrom(const response& from) {
  response* const _this = this;
  // @@protoc_insertion_point(class_specific_merge_from_start:sv_rcon.response)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._impl_._has_bits_[0];
  if (cached_has_bits & 0x0000001fu) {
    if (cached_has_bits & 0x00000001u) {
      _this->_internal_set_responsemsg(from._internal_responsemsg());
    }
    if (cached_has_bits & 0x00000002u) {
      _this->_internal_set_responseval(from._internal_responseval());
    }
    if (cached_has_bits & 0x00000004u) {
      _this->_impl_.messageid_ = from._impl_.messageid_;
    }
    if (cached_has_bits & 0x00000008u) {
      _this->_impl_.messagetype_ = from._impl_.messagetype_;
    }
    if (cached_has_bits & 0x00000010u) {
      _this->_impl_.responsetype_ = from._impl_.responsetype_;
    }
    _this->_impl_._has_bits_[0] |= cached_has_bits;
  }
  _this->_internal_metadata_.MergeFrom<std::string>(from._internal_metadata_);
}

void response::CopyFrom(const response& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:sv_rcon.response)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool response::IsInitialized() const {
  return true;
}

void response::InternalSwap(response* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.responsemsg_, lhs_arena,
      &other->_impl_.responsemsg_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.responseval_, lhs_arena,
      &other->_impl_.responseval_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(response, _impl_.responsetype_)
      + sizeof(response::_impl_.responsetype_)
      - PROTOBUF_FIELD_OFFSET(response, _impl_.messageid_)>(
          reinterpret_cast<char*>(&_impl_.messageid_),
          reinterpret_cast<char*>(&other->_impl_.messageid_));
}

std::string response::GetTypeName() const {
  return "sv_rcon.response";
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace sv_rcon
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::sv_rcon::response*
Arena::CreateMaybeMessage< ::sv_rcon::response >(Arena* arena) {
  return Arena::CreateMessageInternal< ::sv_rcon::response >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <thirdparty/protobuf/port_undef.inc>
