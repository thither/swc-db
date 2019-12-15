/*
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 */

#ifndef swc_lib_fs_Broker_Protocol_params_Readdir_h
#define swc_lib_fs_Broker_Protocol_params_Readdir_h



namespace SWC { namespace FS { namespace Protocol { namespace Params {


class ReaddirReq : public Serializable {
  public:

  ReaddirReq() {}

  ReaddirReq(const std::string& dirname) : dirname(dirname) {}

  std::string dirname;

  private:

  uint8_t encoding_version() const {
    return 1; 
  }

  size_t encoded_length_internal() const override {
    return Serialization::encoded_length_vstr(dirname);
  }

  void encode_internal(uint8_t **bufp) const override {
    Serialization::encode_vstr(bufp, dirname);
  }

  void decode_internal(uint8_t version, const uint8_t **bufp,
			     size_t *remainp) override {
    (void)version;
    dirname.clear();
    dirname.append(Serialization::decode_vstr(bufp, remainp));
  }
};


class ReaddirRsp : public Serializable {
  public:
  
  ReaddirRsp() {}

  ReaddirRsp(DirentList &listing) : m_listing(listing) {}

  void get_listing(DirentList &listing) {
    listing.clear();
    listing.assign(m_listing.begin(), m_listing.end());
  }

  private:

  uint8_t encoding_version() const override {
    return 1;
  }

  size_t encoded_length_internal() const override {
    size_t length = 4;
    for (const Dirent &entry : m_listing)
      length += entry.encoded_length();
    return length;
  }

  void encode_internal(uint8_t **bufp) const override {
    Serialization::encode_i32(bufp, m_listing.size());
    for (const Dirent &entry : m_listing)
      entry.encode(bufp);
  }

  void decode_internal(uint8_t version, const uint8_t **bufp,
	                     size_t *remainp) override {
    (void)version;
    int32_t count = (int32_t)Serialization::decode_i32(bufp, remainp);
    m_listing.reserve(count);
    Dirent entry;
    for (int32_t i=0; i<count; i++) {
      entry.decode(bufp, remainp);
      m_listing.push_back(entry);
    }
  }
  
  DirentList m_listing;
};

}}}}

#endif // swc_lib_fs_Broker_Protocol_params_Readdir_h