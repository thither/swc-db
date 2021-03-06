/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swcdb_db_protocol_rgr_params_RangeLoad_h
#define swcdb_db_protocol_rgr_params_RangeLoad_h

#include "swcdb/db/Protocol/Common/params/ColRangeId.h"

namespace SWC { namespace Comm { namespace Protocol {
namespace Rgr { namespace Params {


class RangeLoad final : public Common::Params::ColRangeId {
  public:

  RangeLoad() noexcept {}

  RangeLoad(cid_t cid, rid_t rid, const DB::Schema::Ptr& schema) noexcept
            : Common::Params::ColRangeId(cid, rid), schema(schema) {
  }

  //~RangeLoad() { }

  DB::Schema::Ptr schema;

  private:

  size_t internal_encoded_length() const override {
    return ColRangeId::internal_encoded_length() + schema->encoded_length();
  }

  void internal_encode(uint8_t** bufp) const override {
    ColRangeId::internal_encode(bufp);
    schema->encode(bufp);
  }

  void internal_decode(const uint8_t** bufp, size_t* remainp) override {
    ColRangeId::internal_decode(bufp, remainp);
    schema = std::make_shared<DB::Schema>(bufp, remainp);
  }

};

class RangeLoaded final : public Serializable {
  public:

  RangeLoaded(const DB::Types::KeySeq key_seq) noexcept
              : intval(false), interval(key_seq) {
  }

  //~RangeLoaded() { }

  bool                intval;
  DB::Cells::Interval interval;

  private:

  size_t internal_encoded_length() const override {
    return 1 + (intval ? interval.encoded_length() : 0);
  }

  void internal_encode(uint8_t** bufp) const override {
    Serialization::encode_bool(bufp, intval);
    if(intval)
      interval.encode(bufp);
  }

  void internal_decode(const uint8_t** bufp, size_t* remainp) override {
    if((intval = Serialization::decode_bool(bufp, remainp)))
      interval.decode(bufp, remainp, false);
  }

};

}}}}}

#endif // swcdb_db_protocol_rgr_params_RangeLoad_h
