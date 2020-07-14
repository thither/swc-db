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



public partial class SpecColumn : TBase
{
  private long _cid;
  private List<SpecInterval> _intervals;

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

  public List<SpecInterval> Intervals
  {
    get
    {
      return _intervals;
    }
    set
    {
      __isset.intervals = true;
      this._intervals = value;
    }
  }


  public Isset __isset;
  public struct Isset
  {
    public bool cid;
    public bool intervals;
  }

  public SpecColumn()
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
            if (field.Type == TType.List)
            {
              {
                TList _list36 = await iprot.ReadListBeginAsync(cancellationToken);
                Intervals = new List<SpecInterval>(_list36.Count);
                for(int _i37 = 0; _i37 < _list36.Count; ++_i37)
                {
                  SpecInterval _elem38;
                  _elem38 = new SpecInterval();
                  await _elem38.ReadAsync(iprot, cancellationToken);
                  Intervals.Add(_elem38);
                }
                await iprot.ReadListEndAsync(cancellationToken);
              }
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
      var struc = new TStruct("SpecColumn");
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
      if (Intervals != null && __isset.intervals)
      {
        field.Name = "intervals";
        field.Type = TType.List;
        field.ID = 2;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        {
          await oprot.WriteListBeginAsync(new TList(TType.Struct, Intervals.Count), cancellationToken);
          foreach (SpecInterval _iter39 in Intervals)
          {
            await _iter39.WriteAsync(oprot, cancellationToken);
          }
          await oprot.WriteListEndAsync(cancellationToken);
        }
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
    var other = that as SpecColumn;
    if (other == null) return false;
    if (ReferenceEquals(this, other)) return true;
    return ((__isset.cid == other.__isset.cid) && ((!__isset.cid) || (System.Object.Equals(Cid, other.Cid))))
      && ((__isset.intervals == other.__isset.intervals) && ((!__isset.intervals) || (TCollections.Equals(Intervals, other.Intervals))));
  }

  public override int GetHashCode() {
    int hashcode = 157;
    unchecked {
      if(__isset.cid)
        hashcode = (hashcode * 397) + Cid.GetHashCode();
      if(__isset.intervals)
        hashcode = (hashcode * 397) + TCollections.GetHashCode(Intervals);
    }
    return hashcode;
  }

  public override string ToString()
  {
    var sb = new StringBuilder("SpecColumn(");
    bool __first = true;
    if (__isset.cid)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Cid: ");
      sb.Append(Cid);
    }
    if (Intervals != null && __isset.intervals)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Intervals: ");
      sb.Append(Intervals);
    }
    sb.Append(")");
    return sb.ToString();
  }
}

