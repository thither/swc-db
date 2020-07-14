/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using Thrift;
using Thrift.Collections;

using Thrift.Protocol;
using Thrift.Protocol.Entities;
using Thrift.Protocol.Utilities;
using Thrift.Transport;
using Thrift.Transport.Client;
using Thrift.Transport.Server;
using Thrift.Processor;



public partial class Schema : TBase
{
  private long _cid;
  private string _col_name;
  private ColumnType _col_type;
  private int _cell_versions;
  private int _cell_ttl;
  private EncodingType _blk_encoding;
  private int _blk_size;
  private int _blk_cells;
  private sbyte _cs_replication;
  private int _cs_size;
  private sbyte _cs_max;
  private sbyte _log_rollout_ratio;
  private sbyte _compact_percent;
  private long _revision;

  public long Cid
  {
    get
    {
      return _cid;
    }
    set
    {
      __isset.cid = true;
      this._cid = value;
    }
  }

  public string Col_name
  {
    get
    {
      return _col_name;
    }
    set
    {
      __isset.col_name = true;
      this._col_name = value;
    }
  }

  /// <summary>
  /// 
  /// <seealso cref="ColumnType"/>
  /// </summary>
  public ColumnType Col_type
  {
    get
    {
      return _col_type;
    }
    set
    {
      __isset.col_type = true;
      this._col_type = value;
    }
  }

  public int Cell_versions
  {
    get
    {
      return _cell_versions;
    }
    set
    {
      __isset.cell_versions = true;
      this._cell_versions = value;
    }
  }

  public int Cell_ttl
  {
    get
    {
      return _cell_ttl;
    }
    set
    {
      __isset.cell_ttl = true;
      this._cell_ttl = value;
    }
  }

  /// <summary>
  /// 
  /// <seealso cref="EncodingType"/>
  /// </summary>
  public EncodingType Blk_encoding
  {
    get
    {
      return _blk_encoding;
    }
    set
    {
      __isset.blk_encoding = true;
      this._blk_encoding = value;
    }
  }

  public int Blk_size
  {
    get
    {
      return _blk_size;
    }
    set
    {
      __isset.blk_size = true;
      this._blk_size = value;
    }
  }

  public int Blk_cells
  {
    get
    {
      return _blk_cells;
    }
    set
    {
      __isset.blk_cells = true;
      this._blk_cells = value;
    }
  }

  public sbyte Cs_replication
  {
    get
    {
      return _cs_replication;
    }
    set
    {
      __isset.cs_replication = true;
      this._cs_replication = value;
    }
  }

  public int Cs_size
  {
    get
    {
      return _cs_size;
    }
    set
    {
      __isset.cs_size = true;
      this._cs_size = value;
    }
  }

  public sbyte Cs_max
  {
    get
    {
      return _cs_max;
    }
    set
    {
      __isset.cs_max = true;
      this._cs_max = value;
    }
  }

  public sbyte Log_rollout_ratio
  {
    get
    {
      return _log_rollout_ratio;
    }
    set
    {
      __isset.log_rollout_ratio = true;
      this._log_rollout_ratio = value;
    }
  }

  public sbyte Compact_percent
  {
    get
    {
      return _compact_percent;
    }
    set
    {
      __isset.compact_percent = true;
      this._compact_percent = value;
    }
  }

  public long Revision
  {
    get
    {
      return _revision;
    }
    set
    {
      __isset.revision = true;
      this._revision = value;
    }
  }


  public Isset __isset;
  public struct Isset
  {
    public bool cid;
    public bool col_name;
    public bool col_type;
    public bool cell_versions;
    public bool cell_ttl;
    public bool blk_encoding;
    public bool blk_size;
    public bool blk_cells;
    public bool cs_replication;
    public bool cs_size;
    public bool cs_max;
    public bool log_rollout_ratio;
    public bool compact_percent;
    public bool revision;
  }

  public Schema()
  {
  }

  public async Task ReadAsync(TProtocol iprot, CancellationToken cancellationToken)
  {
    iprot.IncrementRecursionDepth();
    try
    {
      TField field;
      await iprot.ReadStructBeginAsync(cancellationToken);
      while (true)
      {
        field = await iprot.ReadFieldBeginAsync(cancellationToken);
        if (field.Type == TType.Stop)
        {
          break;
        }

        switch (field.ID)
        {
          case 1:
            if (field.Type == TType.I64)
            {
              Cid = await iprot.ReadI64Async(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 2:
            if (field.Type == TType.String)
            {
              Col_name = await iprot.ReadStringAsync(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 3:
            if (field.Type == TType.I32)
            {
              Col_type = (ColumnType)await iprot.ReadI32Async(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 4:
            if (field.Type == TType.I32)
            {
              Cell_versions = await iprot.ReadI32Async(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 5:
            if (field.Type == TType.I32)
            {
              Cell_ttl = await iprot.ReadI32Async(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 6:
            if (field.Type == TType.I32)
            {
              Blk_encoding = (EncodingType)await iprot.ReadI32Async(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 7:
            if (field.Type == TType.I32)
            {
              Blk_size = await iprot.ReadI32Async(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 8:
            if (field.Type == TType.I32)
            {
              Blk_cells = await iprot.ReadI32Async(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 9:
            if (field.Type == TType.Byte)
            {
              Cs_replication = await iprot.ReadByteAsync(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 10:
            if (field.Type == TType.I32)
            {
              Cs_size = await iprot.ReadI32Async(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 11:
            if (field.Type == TType.Byte)
            {
              Cs_max = await iprot.ReadByteAsync(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 12:
            if (field.Type == TType.Byte)
            {
              Log_rollout_ratio = await iprot.ReadByteAsync(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 13:
            if (field.Type == TType.Byte)
            {
              Compact_percent = await iprot.ReadByteAsync(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 14:
            if (field.Type == TType.I64)
            {
              Revision = await iprot.ReadI64Async(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          default: 
            await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            break;
        }

        await iprot.ReadFieldEndAsync(cancellationToken);
      }

      await iprot.ReadStructEndAsync(cancellationToken);
    }
    finally
    {
      iprot.DecrementRecursionDepth();
    }
  }

  public async Task WriteAsync(TProtocol oprot, CancellationToken cancellationToken)
  {
    oprot.IncrementRecursionDepth();
    try
    {
      var struc = new TStruct("Schema");
      await oprot.WriteStructBeginAsync(struc, cancellationToken);
      var field = new TField();
      if (__isset.cid)
      {
        field.Name = "cid";
        field.Type = TType.I64;
        field.ID = 1;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI64Async(Cid, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (Col_name != null && __isset.col_name)
      {
        field.Name = "col_name";
        field.Type = TType.String;
        field.ID = 2;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteStringAsync(Col_name, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (__isset.col_type)
      {
        field.Name = "col_type";
        field.Type = TType.I32;
        field.ID = 3;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI32Async((int)Col_type, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (__isset.cell_versions)
      {
        field.Name = "cell_versions";
        field.Type = TType.I32;
        field.ID = 4;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI32Async(Cell_versions, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (__isset.cell_ttl)
      {
        field.Name = "cell_ttl";
        field.Type = TType.I32;
        field.ID = 5;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI32Async(Cell_ttl, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (__isset.blk_encoding)
      {
        field.Name = "blk_encoding";
        field.Type = TType.I32;
        field.ID = 6;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI32Async((int)Blk_encoding, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (__isset.blk_size)
      {
        field.Name = "blk_size";
        field.Type = TType.I32;
        field.ID = 7;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI32Async(Blk_size, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (__isset.blk_cells)
      {
        field.Name = "blk_cells";
        field.Type = TType.I32;
        field.ID = 8;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI32Async(Blk_cells, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (__isset.cs_replication)
      {
        field.Name = "cs_replication";
        field.Type = TType.Byte;
        field.ID = 9;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteByteAsync(Cs_replication, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (__isset.cs_size)
      {
        field.Name = "cs_size";
        field.Type = TType.I32;
        field.ID = 10;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI32Async(Cs_size, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (__isset.cs_max)
      {
        field.Name = "cs_max";
        field.Type = TType.Byte;
        field.ID = 11;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteByteAsync(Cs_max, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (__isset.log_rollout_ratio)
      {
        field.Name = "log_rollout_ratio";
        field.Type = TType.Byte;
        field.ID = 12;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteByteAsync(Log_rollout_ratio, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (__isset.compact_percent)
      {
        field.Name = "compact_percent";
        field.Type = TType.Byte;
        field.ID = 13;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteByteAsync(Compact_percent, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (__isset.revision)
      {
        field.Name = "revision";
        field.Type = TType.I64;
        field.ID = 14;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI64Async(Revision, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      await oprot.WriteFieldStopAsync(cancellationToken);
      await oprot.WriteStructEndAsync(cancellationToken);
    }
    finally
    {
      oprot.DecrementRecursionDepth();
    }
  }

  public override bool Equals(object that)
  {
    var other = that as Schema;
    if (other == null) return false;
    if (ReferenceEquals(this, other)) return true;
    return ((__isset.cid == other.__isset.cid) && ((!__isset.cid) || (System.Object.Equals(Cid, other.Cid))))
      && ((__isset.col_name == other.__isset.col_name) && ((!__isset.col_name) || (System.Object.Equals(Col_name, other.Col_name))))
      && ((__isset.col_type == other.__isset.col_type) && ((!__isset.col_type) || (System.Object.Equals(Col_type, other.Col_type))))
      && ((__isset.cell_versions == other.__isset.cell_versions) && ((!__isset.cell_versions) || (System.Object.Equals(Cell_versions, other.Cell_versions))))
      && ((__isset.cell_ttl == other.__isset.cell_ttl) && ((!__isset.cell_ttl) || (System.Object.Equals(Cell_ttl, other.Cell_ttl))))
      && ((__isset.blk_encoding == other.__isset.blk_encoding) && ((!__isset.blk_encoding) || (System.Object.Equals(Blk_encoding, other.Blk_encoding))))
      && ((__isset.blk_size == other.__isset.blk_size) && ((!__isset.blk_size) || (System.Object.Equals(Blk_size, other.Blk_size))))
      && ((__isset.blk_cells == other.__isset.blk_cells) && ((!__isset.blk_cells) || (System.Object.Equals(Blk_cells, other.Blk_cells))))
      && ((__isset.cs_replication == other.__isset.cs_replication) && ((!__isset.cs_replication) || (System.Object.Equals(Cs_replication, other.Cs_replication))))
      && ((__isset.cs_size == other.__isset.cs_size) && ((!__isset.cs_size) || (System.Object.Equals(Cs_size, other.Cs_size))))
      && ((__isset.cs_max == other.__isset.cs_max) && ((!__isset.cs_max) || (System.Object.Equals(Cs_max, other.Cs_max))))
      && ((__isset.log_rollout_ratio == other.__isset.log_rollout_ratio) && ((!__isset.log_rollout_ratio) || (System.Object.Equals(Log_rollout_ratio, other.Log_rollout_ratio))))
      && ((__isset.compact_percent == other.__isset.compact_percent) && ((!__isset.compact_percent) || (System.Object.Equals(Compact_percent, other.Compact_percent))))
      && ((__isset.revision == other.__isset.revision) && ((!__isset.revision) || (System.Object.Equals(Revision, other.Revision))));
  }

  public override int GetHashCode() {
    int hashcode = 157;
    unchecked {
      if(__isset.cid)
        hashcode = (hashcode * 397) + Cid.GetHashCode();
      if(__isset.col_name)
        hashcode = (hashcode * 397) + Col_name.GetHashCode();
      if(__isset.col_type)
        hashcode = (hashcode * 397) + Col_type.GetHashCode();
      if(__isset.cell_versions)
        hashcode = (hashcode * 397) + Cell_versions.GetHashCode();
      if(__isset.cell_ttl)
        hashcode = (hashcode * 397) + Cell_ttl.GetHashCode();
      if(__isset.blk_encoding)
        hashcode = (hashcode * 397) + Blk_encoding.GetHashCode();
      if(__isset.blk_size)
        hashcode = (hashcode * 397) + Blk_size.GetHashCode();
      if(__isset.blk_cells)
        hashcode = (hashcode * 397) + Blk_cells.GetHashCode();
      if(__isset.cs_replication)
        hashcode = (hashcode * 397) + Cs_replication.GetHashCode();
      if(__isset.cs_size)
        hashcode = (hashcode * 397) + Cs_size.GetHashCode();
      if(__isset.cs_max)
        hashcode = (hashcode * 397) + Cs_max.GetHashCode();
      if(__isset.log_rollout_ratio)
        hashcode = (hashcode * 397) + Log_rollout_ratio.GetHashCode();
      if(__isset.compact_percent)
        hashcode = (hashcode * 397) + Compact_percent.GetHashCode();
      if(__isset.revision)
        hashcode = (hashcode * 397) + Revision.GetHashCode();
    }
    return hashcode;
  }

  public override string ToString()
  {
    var sb = new StringBuilder("Schema(");
    bool __first = true;
    if (__isset.cid)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Cid: ");
      sb.Append(Cid);
    }
    if (Col_name != null && __isset.col_name)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Col_name: ");
      sb.Append(Col_name);
    }
    if (__isset.col_type)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Col_type: ");
      sb.Append(Col_type);
    }
    if (__isset.cell_versions)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Cell_versions: ");
      sb.Append(Cell_versions);
    }
    if (__isset.cell_ttl)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Cell_ttl: ");
      sb.Append(Cell_ttl);
    }
    if (__isset.blk_encoding)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Blk_encoding: ");
      sb.Append(Blk_encoding);
    }
    if (__isset.blk_size)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Blk_size: ");
      sb.Append(Blk_size);
    }
    if (__isset.blk_cells)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Blk_cells: ");
      sb.Append(Blk_cells);
    }
    if (__isset.cs_replication)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Cs_replication: ");
      sb.Append(Cs_replication);
    }
    if (__isset.cs_size)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Cs_size: ");
      sb.Append(Cs_size);
    }
    if (__isset.cs_max)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Cs_max: ");
      sb.Append(Cs_max);
    }
    if (__isset.log_rollout_ratio)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Log_rollout_ratio: ");
      sb.Append(Log_rollout_ratio);
    }
    if (__isset.compact_percent)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Compact_percent: ");
      sb.Append(Compact_percent);
    }
    if (__isset.revision)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Revision: ");
      sb.Append(Revision);
    }
    sb.Append(")");
    return sb.ToString();
  }
}

