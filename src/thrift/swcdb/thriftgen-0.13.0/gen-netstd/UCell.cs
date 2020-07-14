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



public partial class UCell : TBase
{
  private Flag _f;
  private List<byte[]> _k;
  private long _ts;
  private bool _ts_desc;
  private byte[] _v;

  /// <summary>
  /// 
  /// <seealso cref="Flag"/>
  /// </summary>
  public Flag F
  {
    get
    {
      return _f;
    }
    set
    {
      __isset.f = true;
      this._f = value;
    }
  }

  public List<byte[]> K
  {
    get
    {
      return _k;
    }
    set
    {
      __isset.k = true;
      this._k = value;
    }
  }

  public long Ts
  {
    get
    {
      return _ts;
    }
    set
    {
      __isset.ts = true;
      this._ts = value;
    }
  }

  public bool Ts_desc
  {
    get
    {
      return _ts_desc;
    }
    set
    {
      __isset.ts_desc = true;
      this._ts_desc = value;
    }
  }

  public byte[] V
  {
    get
    {
      return _v;
    }
    set
    {
      __isset.v = true;
      this._v = value;
    }
  }


  public Isset __isset;
  public struct Isset
  {
    public bool f;
    public bool k;
    public bool ts;
    public bool ts_desc;
    public bool v;
  }

  public UCell()
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
            if (field.Type == TType.I32)
            {
              F = (Flag)await iprot.ReadI32Async(cancellationToken);
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
                TList _list44 = await iprot.ReadListBeginAsync(cancellationToken);
                K = new List<byte[]>(_list44.Count);
                for(int _i45 = 0; _i45 < _list44.Count; ++_i45)
                {
                  byte[] _elem46;
                  _elem46 = await iprot.ReadBinaryAsync(cancellationToken);
                  K.Add(_elem46);
                }
                await iprot.ReadListEndAsync(cancellationToken);
              }
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 3:
            if (field.Type == TType.I64)
            {
              Ts = await iprot.ReadI64Async(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 4:
            if (field.Type == TType.Bool)
            {
              Ts_desc = await iprot.ReadBoolAsync(cancellationToken);
            }
            else
            {
              await TProtocolUtil.SkipAsync(iprot, field.Type, cancellationToken);
            }
            break;
          case 5:
            if (field.Type == TType.String)
            {
              V = await iprot.ReadBinaryAsync(cancellationToken);
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
      var struc = new TStruct("UCell");
      await oprot.WriteStructBeginAsync(struc, cancellationToken);
      var field = new TField();
      if (__isset.f)
      {
        field.Name = "f";
        field.Type = TType.I32;
        field.ID = 1;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI32Async((int)F, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (K != null && __isset.k)
      {
        field.Name = "k";
        field.Type = TType.List;
        field.ID = 2;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        {
          await oprot.WriteListBeginAsync(new TList(TType.String, K.Count), cancellationToken);
          foreach (byte[] _iter47 in K)
          {
            await oprot.WriteBinaryAsync(_iter47, cancellationToken);
          }
          await oprot.WriteListEndAsync(cancellationToken);
        }
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (__isset.ts)
      {
        field.Name = "ts";
        field.Type = TType.I64;
        field.ID = 3;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteI64Async(Ts, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (__isset.ts_desc)
      {
        field.Name = "ts_desc";
        field.Type = TType.Bool;
        field.ID = 4;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteBoolAsync(Ts_desc, cancellationToken);
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if (V != null && __isset.v)
      {
        field.Name = "v";
        field.Type = TType.String;
        field.ID = 5;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        await oprot.WriteBinaryAsync(V, cancellationToken);
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
    var other = that as UCell;
    if (other == null) return false;
    if (ReferenceEquals(this, other)) return true;
    return ((__isset.f == other.__isset.f) && ((!__isset.f) || (System.Object.Equals(F, other.F))))
      && ((__isset.k == other.__isset.k) && ((!__isset.k) || (System.Object.Equals(K, other.K))))
      && ((__isset.ts == other.__isset.ts) && ((!__isset.ts) || (System.Object.Equals(Ts, other.Ts))))
      && ((__isset.ts_desc == other.__isset.ts_desc) && ((!__isset.ts_desc) || (System.Object.Equals(Ts_desc, other.Ts_desc))))
      && ((__isset.v == other.__isset.v) && ((!__isset.v) || (TCollections.Equals(V, other.V))));
  }

  public override int GetHashCode() {
    int hashcode = 157;
    unchecked {
      if(__isset.f)
        hashcode = (hashcode * 397) + F.GetHashCode();
      if(__isset.k)
        hashcode = (hashcode * 397) + K.GetHashCode();
      if(__isset.ts)
        hashcode = (hashcode * 397) + Ts.GetHashCode();
      if(__isset.ts_desc)
        hashcode = (hashcode * 397) + Ts_desc.GetHashCode();
      if(__isset.v)
        hashcode = (hashcode * 397) + V.GetHashCode();
    }
    return hashcode;
  }

  public override string ToString()
  {
    var sb = new StringBuilder("UCell(");
    bool __first = true;
    if (__isset.f)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("F: ");
      sb.Append(F);
    }
    if (K != null && __isset.k)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("K: ");
      sb.Append(K);
    }
    if (__isset.ts)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Ts: ");
      sb.Append(Ts);
    }
    if (__isset.ts_desc)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("Ts_desc: ");
      sb.Append(Ts_desc);
    }
    if (V != null && __isset.v)
    {
      if(!__first) { sb.Append(", "); }
      __first = false;
      sb.Append("V: ");
      sb.Append(V);
    }
    sb.Append(")");
    return sb.ToString();
  }
}

