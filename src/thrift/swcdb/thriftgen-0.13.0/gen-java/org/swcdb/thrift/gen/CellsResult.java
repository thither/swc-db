/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
package org.swcdb.thrift.gen;


public enum CellsResult implements org.apache.thrift.TEnum {
  IN_LIST(0),
  ON_COLUMN(1),
  ON_KEY(2),
  ON_FRACTION(3);

  private final int value;

  private CellsResult(int value) {
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
  public static CellsResult findByValue(int value) { 
    switch (value) {
      case 0:
        return IN_LIST;
      case 1:
        return ON_COLUMN;
      case 2:
        return ON_KEY;
      case 3:
        return ON_FRACTION;
      default:
        return null;
    }
  }
}