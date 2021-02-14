/**
 * Autogenerated by Thrift Compiler (0.14.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.Extensions.Logging;
using Thrift;
using Thrift.Collections;

using Thrift.Protocol;
using Thrift.Protocol.Entities;
using Thrift.Protocol.Utilities;
using Thrift.Transport;
using Thrift.Transport.Client;
using Thrift.Transport.Server;
using Thrift.Processor;


#pragma warning disable IDE0079  // remove unnecessary pragmas
#pragma warning disable IDE1006  // parts of the code use IDL spelling


/// <summary>
/// The Scan Specifications Flags
/// </summary>
public partial class SpecFlags : TBase
{
  private long _limit;
  private long _offset;
  private int _max_versions;
  private int _max_buffer;
  private sbyte _options;

  /// <summary>
  /// Limit to this number of cells
  /// </summary>
  public long Limit
  {
    get
    {
      return _limit;
    }
    set
    {
      __isset.limit = true;
      this._limit = value;
    }
  }

  /// <summary>
  /// Scan from this number of cells Offset on matching Cell-Interval
  /// </summary>
  public long Offset
  {
    get
    {
      return _offset;
    }
    set
    {
      __isset.offset = true;
      this._offset = value;
    }
  }

  /// <summary>
  /// Select only this number of Versions of a given Cell-Key
  /// </summary>
  public int Max_versions
  {
    get
    {
      return _max_versions;
    }
    set
    {
      __isset.max_versions = true;
      this._max_versions = value;
    }
  }

  /// <summary>
  /// return results with reach of this Buffer size in bytes
  /// </summary>
  public int Max_buffer
  {
    get
    {
      return _max_buffer;
    }
    set
    {
      __isset.max_buffer = true;
      this._max_buffer = value;
    }
  }

  /// <summary>
  /// The options bit by SpecFlagsOpt
  /// </summary>
  public sbyte Options
  {
    get
    {
      return _options;
    }
    set
    {
      __isset.options = true;
      this._options = value;
    }
  }


  public Isset __isset;
  public struct Isset
  {
    public bool limit;
    public bool offset;
    public bool max_versions;
    public bool max_buffer;
    public bool options;
  }

  public SpecFlags()
  {
  }

  public SpecFlags DeepCopy()
  {
    var tmp20 = new SpecFlags();
    if(__isset.limit)
    {
      tmp20.Limit = this.Limit;
    }
    tmp20.__isset.limit = this.__isset.limit;
    if(__isset.offset)
    {
      tmp20.Offset = this.Offset;
    }
    tmp20.__isset.offset = this.__isset.offset;
    if(__isset.max_versions)
    {
      tmp20.Max_versions = this.Max_versions;
    }
    tmp20.__isset.max_versions = this.__isset.max_versions;
    if(__isset.max_buffer)
    {
      tmp20.Max_buffer = this.Max_buffer;
    }
    tmp20.__isset.max_buffer = this.__isset.max_buffer;
    if(__isset.options)
    {
      tmp20.Options = this.Options;
    }
    tmp20.__isset.options = this.__isset.options;
    return tmp20;
  }

  public async global::System.Threading.Tasks.Task ReadAsync(TProtocol iprot, CancellationToken cancellationToken)
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
              Limit = await iprot.ReadI64Async(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 2:
            if (field.Type == TType.I64)
            {
              Offset = await iprot.ReadI64Async(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 3:
            if (field.Type == TType.I32)
            {
              Max_versions = await iprot.ReadI32Async(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 4:
            if (field.Type == TType.I32)
            {
              Max_buffer = await iprot.ReadI32Async(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 5:
            if (field.Type == TType.Byte)
            {
              Options = await iprot.ReadByteAsync(cancellationToken);
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

  public async global::System.Threading.Tasks.Task WriteAsync(TProtocol oprot, CancellationToken cancellationToken)
  {
    oprot.IncrementRecursionDepth();
    try
    {
      var struc = new TStruct("SpecFlags");
      await oprot.WriteStructBeginAsync(struc, cancellationToken);
      var field = new TField();
      if(__isset.limit)
      {
        field.Name = "limit";
        field.Type = TType.I64;
        field.ID = 1;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI64Async(Limit, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if(__isset.offset)
      {
        field.Name = "offset";
        field.Type = TType.I64;
        field.ID = 2;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI64Async(Offset, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if(__isset.max_versions)
      {
        field.Name = "max_versions";
        field.Type = TType.I32;
        field.ID = 3;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI32Async(Max_versions, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if(__isset.max_buffer)
      {
        field.Name = "max_buffer";
        field.Type = TType.I32;
        field.ID = 4;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI32Async(Max_buffer, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if(__isset.options)
      {
        field.Name = "options";
        field.Type = TType.Byte;
        field.ID = 5;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteByteAsync(Options, cancellationToken);
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
    if (!(that is SpecFlags other)) return false;
    if (ReferenceEquals(this, other)) return true;
    return ((__isset.limit == other.__isset.limit) && ((!__isset.limit) || (System.Object.Equals(Limit, other.Limit))))
      && ((__isset.offset == other.__isset.offset) && ((!__isset.offset) || (System.Object.Equals(Offset, other.Offset))))
      && ((__isset.max_versions == other.__isset.max_versions) && ((!__isset.max_versions) || (System.Object.Equals(Max_versions, other.Max_versions))))
      && ((__isset.max_buffer == other.__isset.max_buffer) && ((!__isset.max_buffer) || (System.Object.Equals(Max_buffer, other.Max_buffer))))
      && ((__isset.options == other.__isset.options) && ((!__isset.options) || (System.Object.Equals(Options, other.Options))));
  }

  public override int GetHashCode() {
    int hashcode = 157;
    unchecked {
      if(__isset.limit)
      {
        hashcode = (hashcode * 397) + Limit.GetHashCode();
      }
      if(__isset.offset)
      {
        hashcode = (hashcode * 397) + Offset.GetHashCode();
      }
      if(__isset.max_versions)
      {
        hashcode = (hashcode * 397) + Max_versions.GetHashCode();
      }
      if(__isset.max_buffer)
      {
        hashcode = (hashcode * 397) + Max_buffer.GetHashCode();
      }
      if(__isset.options)
      {
        hashcode = (hashcode * 397) + Options.GetHashCode();
      }
    }
    return hashcode;
  }

  public override string ToString()
  {
    var sb = new StringBuilder("SpecFlags(");
    int tmp21 = 0;
    if(__isset.limit)
    {
      if(0 < tmp21++) { sb.Append(", "); }
      sb.Append("Limit: ");
      Limit.ToString(sb);
    }
    if(__isset.offset)
    {
      if(0 < tmp21++) { sb.Append(", "); }
      sb.Append("Offset: ");
      Offset.ToString(sb);
    }
    if(__isset.max_versions)
    {
      if(0 < tmp21++) { sb.Append(", "); }
      sb.Append("Max_versions: ");
      Max_versions.ToString(sb);
    }
    if(__isset.max_buffer)
    {
      if(0 < tmp21++) { sb.Append(", "); }
      sb.Append("Max_buffer: ");
      Max_buffer.ToString(sb);
    }
    if(__isset.options)
    {
      if(0 < tmp21++) { sb.Append(", "); }
      sb.Append("Options: ");
      Options.ToString(sb);
    }
    sb.Append(')');
    return sb.ToString();
  }
}
