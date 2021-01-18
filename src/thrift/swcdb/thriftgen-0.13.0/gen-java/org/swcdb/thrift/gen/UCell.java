/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
package org.swcdb.thrift.gen;

@SuppressWarnings({"cast", "rawtypes", "serial", "unchecked", "unused"})
/**
 * The Cell data for using with Update
 */
public class UCell implements org.apache.thrift.TBase<UCell, UCell._Fields>, java.io.Serializable, Cloneable, Comparable<UCell> {
  private static final org.apache.thrift.protocol.TStruct STRUCT_DESC = new org.apache.thrift.protocol.TStruct("UCell");

  private static final org.apache.thrift.protocol.TField F_FIELD_DESC = new org.apache.thrift.protocol.TField("f", org.apache.thrift.protocol.TType.I32, (short)1);
  private static final org.apache.thrift.protocol.TField K_FIELD_DESC = new org.apache.thrift.protocol.TField("k", org.apache.thrift.protocol.TType.LIST, (short)2);
  private static final org.apache.thrift.protocol.TField TS_FIELD_DESC = new org.apache.thrift.protocol.TField("ts", org.apache.thrift.protocol.TType.I64, (short)3);
  private static final org.apache.thrift.protocol.TField TS_DESC_FIELD_DESC = new org.apache.thrift.protocol.TField("ts_desc", org.apache.thrift.protocol.TType.BOOL, (short)4);
  private static final org.apache.thrift.protocol.TField V_FIELD_DESC = new org.apache.thrift.protocol.TField("v", org.apache.thrift.protocol.TType.STRING, (short)5);
  private static final org.apache.thrift.protocol.TField ENCODER_FIELD_DESC = new org.apache.thrift.protocol.TField("encoder", org.apache.thrift.protocol.TType.I32, (short)6);

  private static final org.apache.thrift.scheme.SchemeFactory STANDARD_SCHEME_FACTORY = new UCellStandardSchemeFactory();
  private static final org.apache.thrift.scheme.SchemeFactory TUPLE_SCHEME_FACTORY = new UCellTupleSchemeFactory();

  /**
   * The Cell Flag
   * 
   * @see Flag
   */
  public @org.apache.thrift.annotation.Nullable Flag f; // required
  /**
   * The Cell Key
   */
  public @org.apache.thrift.annotation.Nullable java.util.List<java.nio.ByteBuffer> k; // required
  /**
   * The Cell Timestamp in nanoseconds
   */
  public long ts; // optional
  /**
   * The Cell Version is in timestamp descending
   */
  public boolean ts_desc; // optional
  /**
   * The Cell Value
   */
  public @org.apache.thrift.annotation.Nullable java.nio.ByteBuffer v; // required
  /**
   * Optionally the Cell Value Encoding Type: ZLIB/SNAPPY/ZSTD
   * 
   * @see EncodingType
   */
  public @org.apache.thrift.annotation.Nullable EncodingType encoder; // optional

  /** The set of fields this struct contains, along with convenience methods for finding and manipulating them. */
  public enum _Fields implements org.apache.thrift.TFieldIdEnum {
    /**
     * The Cell Flag
     * 
     * @see Flag
     */
    F((short)1, "f"),
    /**
     * The Cell Key
     */
    K((short)2, "k"),
    /**
     * The Cell Timestamp in nanoseconds
     */
    TS((short)3, "ts"),
    /**
     * The Cell Version is in timestamp descending
     */
    TS_DESC((short)4, "ts_desc"),
    /**
     * The Cell Value
     */
    V((short)5, "v"),
    /**
     * Optionally the Cell Value Encoding Type: ZLIB/SNAPPY/ZSTD
     * 
     * @see EncodingType
     */
    ENCODER((short)6, "encoder");

    private static final java.util.Map<java.lang.String, _Fields> byName = new java.util.HashMap<java.lang.String, _Fields>();

    static {
      for (_Fields field : java.util.EnumSet.allOf(_Fields.class)) {
        byName.put(field.getFieldName(), field);
      }
    }

    /**
     * Find the _Fields constant that matches fieldId, or null if its not found.
     */
    @org.apache.thrift.annotation.Nullable
    public static _Fields findByThriftId(int fieldId) {
      switch(fieldId) {
        case 1: // F
          return F;
        case 2: // K
          return K;
        case 3: // TS
          return TS;
        case 4: // TS_DESC
          return TS_DESC;
        case 5: // V
          return V;
        case 6: // ENCODER
          return ENCODER;
        default:
          return null;
      }
    }

    /**
     * Find the _Fields constant that matches fieldId, throwing an exception
     * if it is not found.
     */
    public static _Fields findByThriftIdOrThrow(int fieldId) {
      _Fields fields = findByThriftId(fieldId);
      if (fields == null) throw new java.lang.IllegalArgumentException("Field " + fieldId + " doesn't exist!");
      return fields;
    }

    /**
     * Find the _Fields constant that matches name, or null if its not found.
     */
    @org.apache.thrift.annotation.Nullable
    public static _Fields findByName(java.lang.String name) {
      return byName.get(name);
    }

    private final short _thriftId;
    private final java.lang.String _fieldName;

    _Fields(short thriftId, java.lang.String fieldName) {
      _thriftId = thriftId;
      _fieldName = fieldName;
    }

    public short getThriftFieldId() {
      return _thriftId;
    }

    public java.lang.String getFieldName() {
      return _fieldName;
    }
  }

  // isset id assignments
  private static final int __TS_ISSET_ID = 0;
  private static final int __TS_DESC_ISSET_ID = 1;
  private byte __isset_bitfield = 0;
  private static final _Fields optionals[] = {_Fields.TS,_Fields.TS_DESC,_Fields.ENCODER};
  public static final java.util.Map<_Fields, org.apache.thrift.meta_data.FieldMetaData> metaDataMap;
  static {
    java.util.Map<_Fields, org.apache.thrift.meta_data.FieldMetaData> tmpMap = new java.util.EnumMap<_Fields, org.apache.thrift.meta_data.FieldMetaData>(_Fields.class);
    tmpMap.put(_Fields.F, new org.apache.thrift.meta_data.FieldMetaData("f", org.apache.thrift.TFieldRequirementType.DEFAULT, 
        new org.apache.thrift.meta_data.EnumMetaData(org.apache.thrift.protocol.TType.ENUM, Flag.class)));
    tmpMap.put(_Fields.K, new org.apache.thrift.meta_data.FieldMetaData("k", org.apache.thrift.TFieldRequirementType.DEFAULT, 
        new org.apache.thrift.meta_data.FieldValueMetaData(org.apache.thrift.protocol.TType.LIST        , "Key")));
    tmpMap.put(_Fields.TS, new org.apache.thrift.meta_data.FieldMetaData("ts", org.apache.thrift.TFieldRequirementType.OPTIONAL, 
        new org.apache.thrift.meta_data.FieldValueMetaData(org.apache.thrift.protocol.TType.I64)));
    tmpMap.put(_Fields.TS_DESC, new org.apache.thrift.meta_data.FieldMetaData("ts_desc", org.apache.thrift.TFieldRequirementType.OPTIONAL, 
        new org.apache.thrift.meta_data.FieldValueMetaData(org.apache.thrift.protocol.TType.BOOL)));
    tmpMap.put(_Fields.V, new org.apache.thrift.meta_data.FieldMetaData("v", org.apache.thrift.TFieldRequirementType.DEFAULT, 
        new org.apache.thrift.meta_data.FieldValueMetaData(org.apache.thrift.protocol.TType.STRING        , true)));
    tmpMap.put(_Fields.ENCODER, new org.apache.thrift.meta_data.FieldMetaData("encoder", org.apache.thrift.TFieldRequirementType.OPTIONAL, 
        new org.apache.thrift.meta_data.EnumMetaData(org.apache.thrift.protocol.TType.ENUM, EncodingType.class)));
    metaDataMap = java.util.Collections.unmodifiableMap(tmpMap);
    org.apache.thrift.meta_data.FieldMetaData.addStructMetaDataMap(UCell.class, metaDataMap);
  }

  public UCell() {
  }

  public UCell(
    Flag f,
    java.util.List<java.nio.ByteBuffer> k,
    java.nio.ByteBuffer v)
  {
    this();
    this.f = f;
    this.k = k;
    this.v = org.apache.thrift.TBaseHelper.copyBinary(v);
  }

  /**
   * Performs a deep copy on <i>other</i>.
   */
  public UCell(UCell other) {
    __isset_bitfield = other.__isset_bitfield;
    if (other.isSetF()) {
      this.f = other.f;
    }
    if (other.isSetK()) {
      java.util.List<java.nio.ByteBuffer> __this__k = new java.util.ArrayList<java.nio.ByteBuffer>(other.k);
      this.k = __this__k;
    }
    this.ts = other.ts;
    this.ts_desc = other.ts_desc;
    if (other.isSetV()) {
      this.v = org.apache.thrift.TBaseHelper.copyBinary(other.v);
    }
    if (other.isSetEncoder()) {
      this.encoder = other.encoder;
    }
  }

  public UCell deepCopy() {
    return new UCell(this);
  }

  @Override
  public void clear() {
    this.f = null;
    this.k = null;
    setTsIsSet(false);
    this.ts = 0;
    setTs_descIsSet(false);
    this.ts_desc = false;
    this.v = null;
    this.encoder = null;
  }

  /**
   * The Cell Flag
   * 
   * @see Flag
   */
  @org.apache.thrift.annotation.Nullable
  public Flag getF() {
    return this.f;
  }

  /**
   * The Cell Flag
   * 
   * @see Flag
   */
  public UCell setF(@org.apache.thrift.annotation.Nullable Flag f) {
    this.f = f;
    return this;
  }

  public void unsetF() {
    this.f = null;
  }

  /** Returns true if field f is set (has been assigned a value) and false otherwise */
  public boolean isSetF() {
    return this.f != null;
  }

  public void setFIsSet(boolean value) {
    if (!value) {
      this.f = null;
    }
  }

  public int getKSize() {
    return (this.k == null) ? 0 : this.k.size();
  }

  @org.apache.thrift.annotation.Nullable
  public java.util.Iterator<java.nio.ByteBuffer> getKIterator() {
    return (this.k == null) ? null : this.k.iterator();
  }

  public void addToK(java.nio.ByteBuffer elem) {
    if (this.k == null) {
      this.k = new java.util.ArrayList<java.nio.ByteBuffer>();
    }
    this.k.add(elem);
  }

  /**
   * The Cell Key
   */
  @org.apache.thrift.annotation.Nullable
  public java.util.List<java.nio.ByteBuffer> getK() {
    return this.k;
  }

  /**
   * The Cell Key
   */
  public UCell setK(@org.apache.thrift.annotation.Nullable java.util.List<java.nio.ByteBuffer> k) {
    this.k = k;
    return this;
  }

  public void unsetK() {
    this.k = null;
  }

  /** Returns true if field k is set (has been assigned a value) and false otherwise */
  public boolean isSetK() {
    return this.k != null;
  }

  public void setKIsSet(boolean value) {
    if (!value) {
      this.k = null;
    }
  }

  /**
   * The Cell Timestamp in nanoseconds
   */
  public long getTs() {
    return this.ts;
  }

  /**
   * The Cell Timestamp in nanoseconds
   */
  public UCell setTs(long ts) {
    this.ts = ts;
    setTsIsSet(true);
    return this;
  }

  public void unsetTs() {
    __isset_bitfield = org.apache.thrift.EncodingUtils.clearBit(__isset_bitfield, __TS_ISSET_ID);
  }

  /** Returns true if field ts is set (has been assigned a value) and false otherwise */
  public boolean isSetTs() {
    return org.apache.thrift.EncodingUtils.testBit(__isset_bitfield, __TS_ISSET_ID);
  }

  public void setTsIsSet(boolean value) {
    __isset_bitfield = org.apache.thrift.EncodingUtils.setBit(__isset_bitfield, __TS_ISSET_ID, value);
  }

  /**
   * The Cell Version is in timestamp descending
   */
  public boolean isTs_desc() {
    return this.ts_desc;
  }

  /**
   * The Cell Version is in timestamp descending
   */
  public UCell setTs_desc(boolean ts_desc) {
    this.ts_desc = ts_desc;
    setTs_descIsSet(true);
    return this;
  }

  public void unsetTs_desc() {
    __isset_bitfield = org.apache.thrift.EncodingUtils.clearBit(__isset_bitfield, __TS_DESC_ISSET_ID);
  }

  /** Returns true if field ts_desc is set (has been assigned a value) and false otherwise */
  public boolean isSetTs_desc() {
    return org.apache.thrift.EncodingUtils.testBit(__isset_bitfield, __TS_DESC_ISSET_ID);
  }

  public void setTs_descIsSet(boolean value) {
    __isset_bitfield = org.apache.thrift.EncodingUtils.setBit(__isset_bitfield, __TS_DESC_ISSET_ID, value);
  }

  /**
   * The Cell Value
   */
  public byte[] getV() {
    setV(org.apache.thrift.TBaseHelper.rightSize(v));
    return v == null ? null : v.array();
  }

  public java.nio.ByteBuffer bufferForV() {
    return org.apache.thrift.TBaseHelper.copyBinary(v);
  }

  /**
   * The Cell Value
   */
  public UCell setV(byte[] v) {
    this.v = v == null ? (java.nio.ByteBuffer)null   : java.nio.ByteBuffer.wrap(v.clone());
    return this;
  }

  public UCell setV(@org.apache.thrift.annotation.Nullable java.nio.ByteBuffer v) {
    this.v = org.apache.thrift.TBaseHelper.copyBinary(v);
    return this;
  }

  public void unsetV() {
    this.v = null;
  }

  /** Returns true if field v is set (has been assigned a value) and false otherwise */
  public boolean isSetV() {
    return this.v != null;
  }

  public void setVIsSet(boolean value) {
    if (!value) {
      this.v = null;
    }
  }

  /**
   * Optionally the Cell Value Encoding Type: ZLIB/SNAPPY/ZSTD
   * 
   * @see EncodingType
   */
  @org.apache.thrift.annotation.Nullable
  public EncodingType getEncoder() {
    return this.encoder;
  }

  /**
   * Optionally the Cell Value Encoding Type: ZLIB/SNAPPY/ZSTD
   * 
   * @see EncodingType
   */
  public UCell setEncoder(@org.apache.thrift.annotation.Nullable EncodingType encoder) {
    this.encoder = encoder;
    return this;
  }

  public void unsetEncoder() {
    this.encoder = null;
  }

  /** Returns true if field encoder is set (has been assigned a value) and false otherwise */
  public boolean isSetEncoder() {
    return this.encoder != null;
  }

  public void setEncoderIsSet(boolean value) {
    if (!value) {
      this.encoder = null;
    }
  }

  public void setFieldValue(_Fields field, @org.apache.thrift.annotation.Nullable java.lang.Object value) {
    switch (field) {
    case F:
      if (value == null) {
        unsetF();
      } else {
        setF((Flag)value);
      }
      break;

    case K:
      if (value == null) {
        unsetK();
      } else {
        setK((java.util.List<java.nio.ByteBuffer>)value);
      }
      break;

    case TS:
      if (value == null) {
        unsetTs();
      } else {
        setTs((java.lang.Long)value);
      }
      break;

    case TS_DESC:
      if (value == null) {
        unsetTs_desc();
      } else {
        setTs_desc((java.lang.Boolean)value);
      }
      break;

    case V:
      if (value == null) {
        unsetV();
      } else {
        if (value instanceof byte[]) {
          setV((byte[])value);
        } else {
          setV((java.nio.ByteBuffer)value);
        }
      }
      break;

    case ENCODER:
      if (value == null) {
        unsetEncoder();
      } else {
        setEncoder((EncodingType)value);
      }
      break;

    }
  }

  @org.apache.thrift.annotation.Nullable
  public java.lang.Object getFieldValue(_Fields field) {
    switch (field) {
    case F:
      return getF();

    case K:
      return getK();

    case TS:
      return getTs();

    case TS_DESC:
      return isTs_desc();

    case V:
      return getV();

    case ENCODER:
      return getEncoder();

    }
    throw new java.lang.IllegalStateException();
  }

  /** Returns true if field corresponding to fieldID is set (has been assigned a value) and false otherwise */
  public boolean isSet(_Fields field) {
    if (field == null) {
      throw new java.lang.IllegalArgumentException();
    }

    switch (field) {
    case F:
      return isSetF();
    case K:
      return isSetK();
    case TS:
      return isSetTs();
    case TS_DESC:
      return isSetTs_desc();
    case V:
      return isSetV();
    case ENCODER:
      return isSetEncoder();
    }
    throw new java.lang.IllegalStateException();
  }

  @Override
  public boolean equals(java.lang.Object that) {
    if (that == null)
      return false;
    if (that instanceof UCell)
      return this.equals((UCell)that);
    return false;
  }

  public boolean equals(UCell that) {
    if (that == null)
      return false;
    if (this == that)
      return true;

    boolean this_present_f = true && this.isSetF();
    boolean that_present_f = true && that.isSetF();
    if (this_present_f || that_present_f) {
      if (!(this_present_f && that_present_f))
        return false;
      if (!this.f.equals(that.f))
        return false;
    }

    boolean this_present_k = true && this.isSetK();
    boolean that_present_k = true && that.isSetK();
    if (this_present_k || that_present_k) {
      if (!(this_present_k && that_present_k))
        return false;
      if (!this.k.equals(that.k))
        return false;
    }

    boolean this_present_ts = true && this.isSetTs();
    boolean that_present_ts = true && that.isSetTs();
    if (this_present_ts || that_present_ts) {
      if (!(this_present_ts && that_present_ts))
        return false;
      if (this.ts != that.ts)
        return false;
    }

    boolean this_present_ts_desc = true && this.isSetTs_desc();
    boolean that_present_ts_desc = true && that.isSetTs_desc();
    if (this_present_ts_desc || that_present_ts_desc) {
      if (!(this_present_ts_desc && that_present_ts_desc))
        return false;
      if (this.ts_desc != that.ts_desc)
        return false;
    }

    boolean this_present_v = true && this.isSetV();
    boolean that_present_v = true && that.isSetV();
    if (this_present_v || that_present_v) {
      if (!(this_present_v && that_present_v))
        return false;
      if (!this.v.equals(that.v))
        return false;
    }

    boolean this_present_encoder = true && this.isSetEncoder();
    boolean that_present_encoder = true && that.isSetEncoder();
    if (this_present_encoder || that_present_encoder) {
      if (!(this_present_encoder && that_present_encoder))
        return false;
      if (!this.encoder.equals(that.encoder))
        return false;
    }

    return true;
  }

  @Override
  public int hashCode() {
    int hashCode = 1;

    hashCode = hashCode * 8191 + ((isSetF()) ? 131071 : 524287);
    if (isSetF())
      hashCode = hashCode * 8191 + f.getValue();

    hashCode = hashCode * 8191 + ((isSetK()) ? 131071 : 524287);
    if (isSetK())
      hashCode = hashCode * 8191 + k.hashCode();

    hashCode = hashCode * 8191 + ((isSetTs()) ? 131071 : 524287);
    if (isSetTs())
      hashCode = hashCode * 8191 + org.apache.thrift.TBaseHelper.hashCode(ts);

    hashCode = hashCode * 8191 + ((isSetTs_desc()) ? 131071 : 524287);
    if (isSetTs_desc())
      hashCode = hashCode * 8191 + ((ts_desc) ? 131071 : 524287);

    hashCode = hashCode * 8191 + ((isSetV()) ? 131071 : 524287);
    if (isSetV())
      hashCode = hashCode * 8191 + v.hashCode();

    hashCode = hashCode * 8191 + ((isSetEncoder()) ? 131071 : 524287);
    if (isSetEncoder())
      hashCode = hashCode * 8191 + encoder.getValue();

    return hashCode;
  }

  @Override
  public int compareTo(UCell other) {
    if (!getClass().equals(other.getClass())) {
      return getClass().getName().compareTo(other.getClass().getName());
    }

    int lastComparison = 0;

    lastComparison = java.lang.Boolean.valueOf(isSetF()).compareTo(other.isSetF());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetF()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.f, other.f);
      if (lastComparison != 0) {
        return lastComparison;
      }
    }
    lastComparison = java.lang.Boolean.valueOf(isSetK()).compareTo(other.isSetK());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetK()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.k, other.k);
      if (lastComparison != 0) {
        return lastComparison;
      }
    }
    lastComparison = java.lang.Boolean.valueOf(isSetTs()).compareTo(other.isSetTs());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetTs()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.ts, other.ts);
      if (lastComparison != 0) {
        return lastComparison;
      }
    }
    lastComparison = java.lang.Boolean.valueOf(isSetTs_desc()).compareTo(other.isSetTs_desc());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetTs_desc()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.ts_desc, other.ts_desc);
      if (lastComparison != 0) {
        return lastComparison;
      }
    }
    lastComparison = java.lang.Boolean.valueOf(isSetV()).compareTo(other.isSetV());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetV()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.v, other.v);
      if (lastComparison != 0) {
        return lastComparison;
      }
    }
    lastComparison = java.lang.Boolean.valueOf(isSetEncoder()).compareTo(other.isSetEncoder());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetEncoder()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.encoder, other.encoder);
      if (lastComparison != 0) {
        return lastComparison;
      }
    }
    return 0;
  }

  @org.apache.thrift.annotation.Nullable
  public _Fields fieldForId(int fieldId) {
    return _Fields.findByThriftId(fieldId);
  }

  public void read(org.apache.thrift.protocol.TProtocol iprot) throws org.apache.thrift.TException {
    scheme(iprot).read(iprot, this);
  }

  public void write(org.apache.thrift.protocol.TProtocol oprot) throws org.apache.thrift.TException {
    scheme(oprot).write(oprot, this);
  }

  @Override
  public java.lang.String toString() {
    java.lang.StringBuilder sb = new java.lang.StringBuilder("UCell(");
    boolean first = true;

    sb.append("f:");
    if (this.f == null) {
      sb.append("null");
    } else {
      sb.append(this.f);
    }
    first = false;
    if (!first) sb.append(", ");
    sb.append("k:");
    if (this.k == null) {
      sb.append("null");
    } else {
      sb.append(this.k);
    }
    first = false;
    if (isSetTs()) {
      if (!first) sb.append(", ");
      sb.append("ts:");
      sb.append(this.ts);
      first = false;
    }
    if (isSetTs_desc()) {
      if (!first) sb.append(", ");
      sb.append("ts_desc:");
      sb.append(this.ts_desc);
      first = false;
    }
    if (!first) sb.append(", ");
    sb.append("v:");
    if (this.v == null) {
      sb.append("null");
    } else {
      org.apache.thrift.TBaseHelper.toString(this.v, sb);
    }
    first = false;
    if (isSetEncoder()) {
      if (!first) sb.append(", ");
      sb.append("encoder:");
      if (this.encoder == null) {
        sb.append("null");
      } else {
        sb.append(this.encoder);
      }
      first = false;
    }
    sb.append(")");
    return sb.toString();
  }

  public void validate() throws org.apache.thrift.TException {
    // check for required fields
    // check for sub-struct validity
  }

  private void writeObject(java.io.ObjectOutputStream out) throws java.io.IOException {
    try {
      write(new org.apache.thrift.protocol.TCompactProtocol(new org.apache.thrift.transport.TIOStreamTransport(out)));
    } catch (org.apache.thrift.TException te) {
      throw new java.io.IOException(te);
    }
  }

  private void readObject(java.io.ObjectInputStream in) throws java.io.IOException, java.lang.ClassNotFoundException {
    try {
      // it doesn't seem like you should have to do this, but java serialization is wacky, and doesn't call the default constructor.
      __isset_bitfield = 0;
      read(new org.apache.thrift.protocol.TCompactProtocol(new org.apache.thrift.transport.TIOStreamTransport(in)));
    } catch (org.apache.thrift.TException te) {
      throw new java.io.IOException(te);
    }
  }

  private static class UCellStandardSchemeFactory implements org.apache.thrift.scheme.SchemeFactory {
    public UCellStandardScheme getScheme() {
      return new UCellStandardScheme();
    }
  }

  private static class UCellStandardScheme extends org.apache.thrift.scheme.StandardScheme<UCell> {

    public void read(org.apache.thrift.protocol.TProtocol iprot, UCell struct) throws org.apache.thrift.TException {
      org.apache.thrift.protocol.TField schemeField;
      iprot.readStructBegin();
      while (true)
      {
        schemeField = iprot.readFieldBegin();
        if (schemeField.type == org.apache.thrift.protocol.TType.STOP) { 
          break;
        }
        switch (schemeField.id) {
          case 1: // F
            if (schemeField.type == org.apache.thrift.protocol.TType.I32) {
              struct.f = org.swcdb.thrift.gen.Flag.findByValue(iprot.readI32());
              struct.setFIsSet(true);
            } else { 
              org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
            }
            break;
          case 2: // K
            if (schemeField.type == org.apache.thrift.protocol.TType.LIST) {
              {
                org.apache.thrift.protocol.TList _list200 = iprot.readListBegin();
                struct.k = new java.util.ArrayList<java.nio.ByteBuffer>(_list200.size);
                @org.apache.thrift.annotation.Nullable java.nio.ByteBuffer _elem201;
                for (int _i202 = 0; _i202 < _list200.size; ++_i202)
                {
                  _elem201 = iprot.readBinary();
                  struct.k.add(_elem201);
                }
                iprot.readListEnd();
              }
              struct.setKIsSet(true);
            } else { 
              org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
            }
            break;
          case 3: // TS
            if (schemeField.type == org.apache.thrift.protocol.TType.I64) {
              struct.ts = iprot.readI64();
              struct.setTsIsSet(true);
            } else { 
              org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
            }
            break;
          case 4: // TS_DESC
            if (schemeField.type == org.apache.thrift.protocol.TType.BOOL) {
              struct.ts_desc = iprot.readBool();
              struct.setTs_descIsSet(true);
            } else { 
              org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
            }
            break;
          case 5: // V
            if (schemeField.type == org.apache.thrift.protocol.TType.STRING) {
              struct.v = iprot.readBinary();
              struct.setVIsSet(true);
            } else { 
              org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
            }
            break;
          case 6: // ENCODER
            if (schemeField.type == org.apache.thrift.protocol.TType.I32) {
              struct.encoder = org.swcdb.thrift.gen.EncodingType.findByValue(iprot.readI32());
              struct.setEncoderIsSet(true);
            } else { 
              org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
            }
            break;
          default:
            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
        }
        iprot.readFieldEnd();
      }
      iprot.readStructEnd();

      // check for required fields of primitive type, which can't be checked in the validate method
      struct.validate();
    }

    public void write(org.apache.thrift.protocol.TProtocol oprot, UCell struct) throws org.apache.thrift.TException {
      struct.validate();

      oprot.writeStructBegin(STRUCT_DESC);
      if (struct.f != null) {
        oprot.writeFieldBegin(F_FIELD_DESC);
        oprot.writeI32(struct.f.getValue());
        oprot.writeFieldEnd();
      }
      if (struct.k != null) {
        oprot.writeFieldBegin(K_FIELD_DESC);
        {
          oprot.writeListBegin(new org.apache.thrift.protocol.TList(org.apache.thrift.protocol.TType.STRING, struct.k.size()));
          for (java.nio.ByteBuffer _iter203 : struct.k)
          {
            oprot.writeBinary(_iter203);
          }
          oprot.writeListEnd();
        }
        oprot.writeFieldEnd();
      }
      if (struct.isSetTs()) {
        oprot.writeFieldBegin(TS_FIELD_DESC);
        oprot.writeI64(struct.ts);
        oprot.writeFieldEnd();
      }
      if (struct.isSetTs_desc()) {
        oprot.writeFieldBegin(TS_DESC_FIELD_DESC);
        oprot.writeBool(struct.ts_desc);
        oprot.writeFieldEnd();
      }
      if (struct.v != null) {
        oprot.writeFieldBegin(V_FIELD_DESC);
        oprot.writeBinary(struct.v);
        oprot.writeFieldEnd();
      }
      if (struct.encoder != null) {
        if (struct.isSetEncoder()) {
          oprot.writeFieldBegin(ENCODER_FIELD_DESC);
          oprot.writeI32(struct.encoder.getValue());
          oprot.writeFieldEnd();
        }
      }
      oprot.writeFieldStop();
      oprot.writeStructEnd();
    }

  }

  private static class UCellTupleSchemeFactory implements org.apache.thrift.scheme.SchemeFactory {
    public UCellTupleScheme getScheme() {
      return new UCellTupleScheme();
    }
  }

  private static class UCellTupleScheme extends org.apache.thrift.scheme.TupleScheme<UCell> {

    @Override
    public void write(org.apache.thrift.protocol.TProtocol prot, UCell struct) throws org.apache.thrift.TException {
      org.apache.thrift.protocol.TTupleProtocol oprot = (org.apache.thrift.protocol.TTupleProtocol) prot;
      java.util.BitSet optionals = new java.util.BitSet();
      if (struct.isSetF()) {
        optionals.set(0);
      }
      if (struct.isSetK()) {
        optionals.set(1);
      }
      if (struct.isSetTs()) {
        optionals.set(2);
      }
      if (struct.isSetTs_desc()) {
        optionals.set(3);
      }
      if (struct.isSetV()) {
        optionals.set(4);
      }
      if (struct.isSetEncoder()) {
        optionals.set(5);
      }
      oprot.writeBitSet(optionals, 6);
      if (struct.isSetF()) {
        oprot.writeI32(struct.f.getValue());
      }
      if (struct.isSetK()) {
        {
          oprot.writeI32(struct.k.size());
          for (java.nio.ByteBuffer _iter204 : struct.k)
          {
            oprot.writeBinary(_iter204);
          }
        }
      }
      if (struct.isSetTs()) {
        oprot.writeI64(struct.ts);
      }
      if (struct.isSetTs_desc()) {
        oprot.writeBool(struct.ts_desc);
      }
      if (struct.isSetV()) {
        oprot.writeBinary(struct.v);
      }
      if (struct.isSetEncoder()) {
        oprot.writeI32(struct.encoder.getValue());
      }
    }

    @Override
    public void read(org.apache.thrift.protocol.TProtocol prot, UCell struct) throws org.apache.thrift.TException {
      org.apache.thrift.protocol.TTupleProtocol iprot = (org.apache.thrift.protocol.TTupleProtocol) prot;
      java.util.BitSet incoming = iprot.readBitSet(6);
      if (incoming.get(0)) {
        struct.f = org.swcdb.thrift.gen.Flag.findByValue(iprot.readI32());
        struct.setFIsSet(true);
      }
      if (incoming.get(1)) {
        {
          org.apache.thrift.protocol.TList _list205 = new org.apache.thrift.protocol.TList(org.apache.thrift.protocol.TType.STRING, iprot.readI32());
          struct.k = new java.util.ArrayList<java.nio.ByteBuffer>(_list205.size);
          @org.apache.thrift.annotation.Nullable java.nio.ByteBuffer _elem206;
          for (int _i207 = 0; _i207 < _list205.size; ++_i207)
          {
            _elem206 = iprot.readBinary();
            struct.k.add(_elem206);
          }
        }
        struct.setKIsSet(true);
      }
      if (incoming.get(2)) {
        struct.ts = iprot.readI64();
        struct.setTsIsSet(true);
      }
      if (incoming.get(3)) {
        struct.ts_desc = iprot.readBool();
        struct.setTs_descIsSet(true);
      }
      if (incoming.get(4)) {
        struct.v = iprot.readBinary();
        struct.setVIsSet(true);
      }
      if (incoming.get(5)) {
        struct.encoder = org.swcdb.thrift.gen.EncodingType.findByValue(iprot.readI32());
        struct.setEncoderIsSet(true);
      }
    }
  }

  private static <S extends org.apache.thrift.scheme.IScheme> S scheme(org.apache.thrift.protocol.TProtocol proto) {
    return (org.apache.thrift.scheme.StandardScheme.class.equals(proto.getScheme()) ? STANDARD_SCHEME_FACTORY : TUPLE_SCHEME_FACTORY).getScheme();
  }
}

