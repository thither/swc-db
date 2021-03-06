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
/// The Fraction Cells for results on Fraction of scan
/// </summary>
public partial class FCells : TBase
{
  private Dictionary<byte[], FCells> _f;
  private List<FCell> _cells;
  private List<FCellSerial> _serial_cells;

  /// <summary>
  /// The Fraction Container for the Next Fractions Tree,  defined as FCells items in a map-container by current Fraction bytes
  /// </summary>
  public Dictionary<byte[], FCells> F
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

  /// <summary>
  /// The current Fraction's Cells, defined as FCell items in a list-container
  /// </summary>
  public List<FCell> Cells
  {
    get
    {
      return _cells;
    }
    set
    {
      __isset.cells = true;
      this._cells = value;
    }
  }

  /// <summary>
  /// The current Fraction's Serial Cells, defined as FCellSerial items in a list-container
  /// </summary>
  public List<FCellSerial> Serial_cells
  {
    get
    {
      return _serial_cells;
    }
    set
    {
      __isset.serial_cells = true;
      this._serial_cells = value;
    }
  }


  public Isset __isset;
  public struct Isset
  {
    public bool f;
    public bool cells;
    public bool serial_cells;
  }

  public FCells()
  {
  }

  public FCells DeepCopy()
  {
    var tmp248 = new FCells();
    if((F != null) && __isset.f)
    {
      tmp248.F = this.F.DeepCopy();
    }
    tmp248.__isset.f = this.__isset.f;
    if((Cells != null) && __isset.cells)
    {
      tmp248.Cells = this.Cells.DeepCopy();
    }
    tmp248.__isset.cells = this.__isset.cells;
    if((Serial_cells != null) && __isset.serial_cells)
    {
      tmp248.Serial_cells = this.Serial_cells.DeepCopy();
    }
    tmp248.__isset.serial_cells = this.__isset.serial_cells;
    return tmp248;
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
            if (field.Type == TType.Map)
            {
              {
                TMap _map249 = await iprot.ReadMapBeginAsync(cancellationToken);
                F = new Dictionary<byte[], FCells>(_map249.Count);
                for(int _i250 = 0; _i250 < _map249.Count; ++_i250)
                {
                  byte[] _key251;
                  FCells _val252;
                  _key251 = await iprot.ReadBinaryAsync(cancellationToken);
                  _val252 = new FCells();
                  await _val252.ReadAsync(iprot, cancellationToken);
                  F[_key251] = _val252;
                }
                await iprot.ReadMapEndAsync(cancellationToken);
              }
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
                TList _list253 = await iprot.ReadListBeginAsync(cancellationToken);
                Cells = new List<FCell>(_list253.Count);
                for(int _i254 = 0; _i254 < _list253.Count; ++_i254)
                {
                  FCell _elem255;
                  _elem255 = new FCell();
                  await _elem255.ReadAsync(iprot, cancellationToken);
                  Cells.Add(_elem255);
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
            if (field.Type == TType.List)
            {
              {
                TList _list256 = await iprot.ReadListBeginAsync(cancellationToken);
                Serial_cells = new List<FCellSerial>(_list256.Count);
                for(int _i257 = 0; _i257 < _list256.Count; ++_i257)
                {
                  FCellSerial _elem258;
                  _elem258 = new FCellSerial();
                  await _elem258.ReadAsync(iprot, cancellationToken);
                  Serial_cells.Add(_elem258);
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

  public async global::System.Threading.Tasks.Task WriteAsync(TProtocol oprot, CancellationToken cancellationToken)
  {
    oprot.IncrementRecursionDepth();
    try
    {
      var struc = new TStruct("FCells");
      await oprot.WriteStructBeginAsync(struc, cancellationToken);
      var field = new TField();
      if((F != null) && __isset.f)
      {
        field.Name = "f";
        field.Type = TType.Map;
        field.ID = 1;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        {
          await oprot.WriteMapBeginAsync(new TMap(TType.String, TType.Struct, F.Count), cancellationToken);
          foreach (byte[] _iter259 in F.Keys)
          {
            await oprot.WriteBinaryAsync(_iter259, cancellationToken);
            await F[_iter259].WriteAsync(oprot, cancellationToken);
          }
          await oprot.WriteMapEndAsync(cancellationToken);
        }
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if((Cells != null) && __isset.cells)
      {
        field.Name = "cells";
        field.Type = TType.List;
        field.ID = 2;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        {
          await oprot.WriteListBeginAsync(new TList(TType.Struct, Cells.Count), cancellationToken);
          foreach (FCell _iter260 in Cells)
          {
            await _iter260.WriteAsync(oprot, cancellationToken);
          }
          await oprot.WriteListEndAsync(cancellationToken);
        }
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if((Serial_cells != null) && __isset.serial_cells)
      {
        field.Name = "serial_cells";
        field.Type = TType.List;
        field.ID = 3;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        {
          await oprot.WriteListBeginAsync(new TList(TType.Struct, Serial_cells.Count), cancellationToken);
          foreach (FCellSerial _iter261 in Serial_cells)
          {
            await _iter261.WriteAsync(oprot, cancellationToken);
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
    if (!(that is FCells other)) return false;
    if (ReferenceEquals(this, other)) return true;
    return ((__isset.f == other.__isset.f) && ((!__isset.f) || (TCollections.Equals(F, other.F))))
      && ((__isset.cells == other.__isset.cells) && ((!__isset.cells) || (TCollections.Equals(Cells, other.Cells))))
      && ((__isset.serial_cells == other.__isset.serial_cells) && ((!__isset.serial_cells) || (TCollections.Equals(Serial_cells, other.Serial_cells))));
  }

  public override int GetHashCode() {
    int hashcode = 157;
    unchecked {
      if((F != null) && __isset.f)
      {
        hashcode = (hashcode * 397) + TCollections.GetHashCode(F);
      }
      if((Cells != null) && __isset.cells)
      {
        hashcode = (hashcode * 397) + TCollections.GetHashCode(Cells);
      }
      if((Serial_cells != null) && __isset.serial_cells)
      {
        hashcode = (hashcode * 397) + TCollections.GetHashCode(Serial_cells);
      }
    }
    return hashcode;
  }

  public override string ToString()
  {
    var sb = new StringBuilder("FCells(");
    int tmp262 = 0;
    if((F != null) && __isset.f)
    {
      if(0 < tmp262++) { sb.Append(", "); }
      sb.Append("F: ");
      F.ToString(sb);
    }
    if((Cells != null) && __isset.cells)
    {
      if(0 < tmp262++) { sb.Append(", "); }
      sb.Append("Cells: ");
      Cells.ToString(sb);
    }
    if((Serial_cells != null) && __isset.serial_cells)
    {
      if(0 < tmp262++) { sb.Append(", "); }
      sb.Append("Serial_cells: ");
      Serial_cells.ToString(sb);
    }
    sb.Append(')');
    return sb.ToString();
  }
}

