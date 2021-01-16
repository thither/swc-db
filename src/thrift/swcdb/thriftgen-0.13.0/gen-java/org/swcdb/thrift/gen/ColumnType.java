/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
package org.swcdb.thrift.gen;


/**
 * Column Value Types
 */
public enum ColumnType implements org.apache.thrift.TEnum {
  /**
   * Unknown/Unrecognized Type
   */
  UNKNOWN(0),
  /**
   * A Plain Column Value
   */
  PLAIN(1),
  /**
   * A Counter Column Value with integrity of signed-64bit
   */
  COUNTER_I64(2),
  /**
   * A Counter Column Value with integrity of signed-32bit
   */
  COUNTER_I32(3),
  /**
   * A Counter Column Value with integrity of signed-16bit
   */
  COUNTER_I16(4),
  /**
   * A Counter Column Value with integrity of signed-8bit
   */
  COUNTER_I8(5),
  /**
   * A Serial Column Value
   */
  SERIAL(6),
  /**
   * Not used - experimental
   */
  CELL_DEFINED(15);

  private final int value;

  private ColumnType(int value) {
    this.value = value;
  }

  /**
   * Get the integer value of this enum value, as defined in the Thrift IDL.
   */
  public int getValue() {
    return value;
  }

  /**
   * Find a the enum type by its integer value, as defined in the Thrift IDL.
   * @return null if the value is not found.
   */
  @org.apache.thrift.annotation.Nullable
  public static ColumnType findByValue(int value) { 
    switch (value) {
      case 0:
        return UNKNOWN;
      case 1:
        return PLAIN;
      case 2:
        return COUNTER_I64;
      case 3:
        return COUNTER_I32;
      case 4:
        return COUNTER_I16;
      case 5:
        return COUNTER_I8;
      case 6:
        return SERIAL;
      case 15:
        return CELL_DEFINED;
      default:
        return null;
    }
  }
}
