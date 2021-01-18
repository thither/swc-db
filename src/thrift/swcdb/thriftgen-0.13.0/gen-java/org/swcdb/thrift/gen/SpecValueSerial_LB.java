/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
package org.swcdb.thrift.gen;

@SuppressWarnings({"cast", "rawtypes", "serial", "unchecked", "unused"})
/**
 * The Specifications of LIST_BYTES(LB) Serial Value Field
 */
public class SpecValueSerial_LB implements org.apache.thrift.TBase<SpecValueSerial_LB, SpecValueSerial_LB._Fields>, java.io.Serializable, Cloneable, Comparable<SpecValueSerial_LB> {
  private static final org.apache.thrift.protocol.TStruct STRUCT_DESC = new org.apache.thrift.protocol.TStruct("SpecValueSerial_LB");

  private static final org.apache.thrift.protocol.TField COMP_FIELD_DESC = new org.apache.thrift.protocol.TField("comp", org.apache.thrift.protocol.TType.I32, (short)1);
  private static final org.apache.thrift.protocol.TField V_FIELD_DESC = new org.apache.thrift.protocol.TField("v", org.apache.thrift.protocol.TType.LIST, (short)2);

  private static final org.apache.thrift.scheme.SchemeFactory STANDARD_SCHEME_FACTORY = new SpecValueSerial_LBStandardSchemeFactory();
  private static final org.apache.thrift.scheme.SchemeFactory TUPLE_SCHEME_FACTORY = new SpecValueSerial_LBTupleSchemeFactory();

  /**
   * Logical comparator to Apply
   * 
   * @see Comp
   */
  public @org.apache.thrift.annotation.Nullable Comp comp; // required
  /**
   * The List of Bytes to match against the value field
   */
  public @org.apache.thrift.annotation.Nullable java.util.List<SpecValueSerial_BYTES> v; // required

  /** The set of fields this struct contains, along with convenience methods for finding and manipulating them. */
  public enum _Fields implements org.apache.thrift.TFieldIdEnum {
    /**
     * Logical comparator to Apply
     * 
     * @see Comp
     */
    COMP((short)1, "comp"),
    /**
     * The List of Bytes to match against the value field
     */
    V((short)2, "v");

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
        case 1: // COMP
          return COMP;
        case 2: // V
          return V;
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
  public static final java.util.Map<_Fields, org.apache.thrift.meta_data.FieldMetaData> metaDataMap;
  static {
    java.util.Map<_Fields, org.apache.thrift.meta_data.FieldMetaData> tmpMap = new java.util.EnumMap<_Fields, org.apache.thrift.meta_data.FieldMetaData>(_Fields.class);
    tmpMap.put(_Fields.COMP, new org.apache.thrift.meta_data.FieldMetaData("comp", org.apache.thrift.TFieldRequirementType.DEFAULT, 
        new org.apache.thrift.meta_data.EnumMetaData(org.apache.thrift.protocol.TType.ENUM, Comp.class)));
    tmpMap.put(_Fields.V, new org.apache.thrift.meta_data.FieldMetaData("v", org.apache.thrift.TFieldRequirementType.DEFAULT, 
        new org.apache.thrift.meta_data.ListMetaData(org.apache.thrift.protocol.TType.LIST, 
            new org.apache.thrift.meta_data.StructMetaData(org.apache.thrift.protocol.TType.STRUCT, SpecValueSerial_BYTES.class))));
    metaDataMap = java.util.Collections.unmodifiableMap(tmpMap);
    org.apache.thrift.meta_data.FieldMetaData.addStructMetaDataMap(SpecValueSerial_LB.class, metaDataMap);
  }

  public SpecValueSerial_LB() {
  }

  public SpecValueSerial_LB(
    Comp comp,
    java.util.List<SpecValueSerial_BYTES> v)
  {
    this();
    this.comp = comp;
    this.v = v;
  }

  /**
   * Performs a deep copy on <i>other</i>.
   */
  public SpecValueSerial_LB(SpecValueSerial_LB other) {
    if (other.isSetComp()) {
      this.comp = other.comp;
    }
    if (other.isSetV()) {
      java.util.List<SpecValueSerial_BYTES> __this__v = new java.util.ArrayList<SpecValueSerial_BYTES>(other.v.size());
      for (SpecValueSerial_BYTES other_element : other.v) {
        __this__v.add(new SpecValueSerial_BYTES(other_element));
      }
      this.v = __this__v;
    }
  }

  public SpecValueSerial_LB deepCopy() {
    return new SpecValueSerial_LB(this);
  }

  @Override
  public void clear() {
    this.comp = null;
    this.v = null;
  }

  /**
   * Logical comparator to Apply
   * 
   * @see Comp
   */
  @org.apache.thrift.annotation.Nullable
  public Comp getComp() {
    return this.comp;
  }

  /**
   * Logical comparator to Apply
   * 
   * @see Comp
   */
  public SpecValueSerial_LB setComp(@org.apache.thrift.annotation.Nullable Comp comp) {
    this.comp = comp;
    return this;
  }

  public void unsetComp() {
    this.comp = null;
  }

  /** Returns true if field comp is set (has been assigned a value) and false otherwise */
  public boolean isSetComp() {
    return this.comp != null;
  }

  public void setCompIsSet(boolean value) {
    if (!value) {
      this.comp = null;
    }
  }

  public int getVSize() {
    return (this.v == null) ? 0 : this.v.size();
  }

  @org.apache.thrift.annotation.Nullable
  public java.util.Iterator<SpecValueSerial_BYTES> getVIterator() {
    return (this.v == null) ? null : this.v.iterator();
  }

  public void addToV(SpecValueSerial_BYTES elem) {
    if (this.v == null) {
      this.v = new java.util.ArrayList<SpecValueSerial_BYTES>();
    }
    this.v.add(elem);
  }

  /**
   * The List of Bytes to match against the value field
   */
  @org.apache.thrift.annotation.Nullable
  public java.util.List<SpecValueSerial_BYTES> getV() {
    return this.v;
  }

  /**
   * The List of Bytes to match against the value field
   */
  public SpecValueSerial_LB setV(@org.apache.thrift.annotation.Nullable java.util.List<SpecValueSerial_BYTES> v) {
    this.v = v;
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

  public void setFieldValue(_Fields field, @org.apache.thrift.annotation.Nullable java.lang.Object value) {
    switch (field) {
    case COMP:
      if (value == null) {
        unsetComp();
      } else {
        setComp((Comp)value);
      }
      break;

    case V:
      if (value == null) {
        unsetV();
      } else {
        setV((java.util.List<SpecValueSerial_BYTES>)value);
      }
      break;

    }
  }

  @org.apache.thrift.annotation.Nullable
  public java.lang.Object getFieldValue(_Fields field) {
    switch (field) {
    case COMP:
      return getComp();

    case V:
      return getV();

    }
    throw new java.lang.IllegalStateException();
  }

  /** Returns true if field corresponding to fieldID is set (has been assigned a value) and false otherwise */
  public boolean isSet(_Fields field) {
    if (field == null) {
      throw new java.lang.IllegalArgumentException();
    }

    switch (field) {
    case COMP:
      return isSetComp();
    case V:
      return isSetV();
    }
    throw new java.lang.IllegalStateException();
  }

  @Override
  public boolean equals(java.lang.Object that) {
    if (that == null)
      return false;
    if (that instanceof SpecValueSerial_LB)
      return this.equals((SpecValueSerial_LB)that);
    return false;
  }

  public boolean equals(SpecValueSerial_LB that) {
    if (that == null)
      return false;
    if (this == that)
      return true;

    boolean this_present_comp = true && this.isSetComp();
    boolean that_present_comp = true && that.isSetComp();
    if (this_present_comp || that_present_comp) {
      if (!(this_present_comp && that_present_comp))
        return false;
      if (!this.comp.equals(that.comp))
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

    return true;
  }

  @Override
  public int hashCode() {
    int hashCode = 1;

    hashCode = hashCode * 8191 + ((isSetComp()) ? 131071 : 524287);
    if (isSetComp())
      hashCode = hashCode * 8191 + comp.getValue();

    hashCode = hashCode * 8191 + ((isSetV()) ? 131071 : 524287);
    if (isSetV())
      hashCode = hashCode * 8191 + v.hashCode();

    return hashCode;
  }

  @Override
  public int compareTo(SpecValueSerial_LB other) {
    if (!getClass().equals(other.getClass())) {
      return getClass().getName().compareTo(other.getClass().getName());
    }

    int lastComparison = 0;

    lastComparison = java.lang.Boolean.valueOf(isSetComp()).compareTo(other.isSetComp());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetComp()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.comp, other.comp);
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
    java.lang.StringBuilder sb = new java.lang.StringBuilder("SpecValueSerial_LB(");
    boolean first = true;

    sb.append("comp:");
    if (this.comp == null) {
      sb.append("null");
    } else {
      sb.append(this.comp);
    }
    first = false;
    if (!first) sb.append(", ");
    sb.append("v:");
    if (this.v == null) {
      sb.append("null");
    } else {
      sb.append(this.v);
    }
    first = false;
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
      read(new org.apache.thrift.protocol.TCompactProtocol(new org.apache.thrift.transport.TIOStreamTransport(in)));
    } catch (org.apache.thrift.TException te) {
      throw new java.io.IOException(te);
    }
  }

  private static class SpecValueSerial_LBStandardSchemeFactory implements org.apache.thrift.scheme.SchemeFactory {
    public SpecValueSerial_LBStandardScheme getScheme() {
      return new SpecValueSerial_LBStandardScheme();
    }
  }

  private static class SpecValueSerial_LBStandardScheme extends org.apache.thrift.scheme.StandardScheme<SpecValueSerial_LB> {

    public void read(org.apache.thrift.protocol.TProtocol iprot, SpecValueSerial_LB struct) throws org.apache.thrift.TException {
      org.apache.thrift.protocol.TField schemeField;
      iprot.readStructBegin();
      while (true)
      {
        schemeField = iprot.readFieldBegin();
        if (schemeField.type == org.apache.thrift.protocol.TType.STOP) { 
          break;
        }
        switch (schemeField.id) {
          case 1: // COMP
            if (schemeField.type == org.apache.thrift.protocol.TType.I32) {
              struct.comp = org.swcdb.thrift.gen.Comp.findByValue(iprot.readI32());
              struct.setCompIsSet(true);
            } else { 
              org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
            }
            break;
          case 2: // V
            if (schemeField.type == org.apache.thrift.protocol.TType.LIST) {
              {
                org.apache.thrift.protocol.TList _list112 = iprot.readListBegin();
                struct.v = new java.util.ArrayList<SpecValueSerial_BYTES>(_list112.size);
                @org.apache.thrift.annotation.Nullable SpecValueSerial_BYTES _elem113;
                for (int _i114 = 0; _i114 < _list112.size; ++_i114)
                {
                  _elem113 = new SpecValueSerial_BYTES();
                  _elem113.read(iprot);
                  struct.v.add(_elem113);
                }
                iprot.readListEnd();
              }
              struct.setVIsSet(true);
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

    public void write(org.apache.thrift.protocol.TProtocol oprot, SpecValueSerial_LB struct) throws org.apache.thrift.TException {
      struct.validate();

      oprot.writeStructBegin(STRUCT_DESC);
      if (struct.comp != null) {
        oprot.writeFieldBegin(COMP_FIELD_DESC);
        oprot.writeI32(struct.comp.getValue());
        oprot.writeFieldEnd();
      }
      if (struct.v != null) {
        oprot.writeFieldBegin(V_FIELD_DESC);
        {
          oprot.writeListBegin(new org.apache.thrift.protocol.TList(org.apache.thrift.protocol.TType.STRUCT, struct.v.size()));
          for (SpecValueSerial_BYTES _iter115 : struct.v)
          {
            _iter115.write(oprot);
          }
          oprot.writeListEnd();
        }
        oprot.writeFieldEnd();
      }
      oprot.writeFieldStop();
      oprot.writeStructEnd();
    }

  }

  private static class SpecValueSerial_LBTupleSchemeFactory implements org.apache.thrift.scheme.SchemeFactory {
    public SpecValueSerial_LBTupleScheme getScheme() {
      return new SpecValueSerial_LBTupleScheme();
    }
  }

  private static class SpecValueSerial_LBTupleScheme extends org.apache.thrift.scheme.TupleScheme<SpecValueSerial_LB> {

    @Override
    public void write(org.apache.thrift.protocol.TProtocol prot, SpecValueSerial_LB struct) throws org.apache.thrift.TException {
      org.apache.thrift.protocol.TTupleProtocol oprot = (org.apache.thrift.protocol.TTupleProtocol) prot;
      java.util.BitSet optionals = new java.util.BitSet();
      if (struct.isSetComp()) {
        optionals.set(0);
      }
      if (struct.isSetV()) {
        optionals.set(1);
      }
      oprot.writeBitSet(optionals, 2);
      if (struct.isSetComp()) {
        oprot.writeI32(struct.comp.getValue());
      }
      if (struct.isSetV()) {
        {
          oprot.writeI32(struct.v.size());
          for (SpecValueSerial_BYTES _iter116 : struct.v)
          {
            _iter116.write(oprot);
          }
        }
      }
    }

    @Override
    public void read(org.apache.thrift.protocol.TProtocol prot, SpecValueSerial_LB struct) throws org.apache.thrift.TException {
      org.apache.thrift.protocol.TTupleProtocol iprot = (org.apache.thrift.protocol.TTupleProtocol) prot;
      java.util.BitSet incoming = iprot.readBitSet(2);
      if (incoming.get(0)) {
        struct.comp = org.swcdb.thrift.gen.Comp.findByValue(iprot.readI32());
        struct.setCompIsSet(true);
      }
      if (incoming.get(1)) {
        {
          org.apache.thrift.protocol.TList _list117 = new org.apache.thrift.protocol.TList(org.apache.thrift.protocol.TType.STRUCT, iprot.readI32());
          struct.v = new java.util.ArrayList<SpecValueSerial_BYTES>(_list117.size);
          @org.apache.thrift.annotation.Nullable SpecValueSerial_BYTES _elem118;
          for (int _i119 = 0; _i119 < _list117.size; ++_i119)
          {
            _elem118 = new SpecValueSerial_BYTES();
            _elem118.read(iprot);
            struct.v.add(_elem118);
          }
        }
        struct.setVIsSet(true);
      }
    }
  }

  private static <S extends org.apache.thrift.scheme.IScheme> S scheme(org.apache.thrift.protocol.TProtocol proto) {
    return (org.apache.thrift.scheme.StandardScheme.class.equals(proto.getScheme()) ? STANDARD_SCHEME_FACTORY : TUPLE_SCHEME_FACTORY).getScheme();
  }
}

