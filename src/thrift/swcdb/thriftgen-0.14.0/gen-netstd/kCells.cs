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
/// The Key Cells for results on Key of scan
/// </summary>
public partial class kCells : TBase
{
  private List<byte[]> _k;
  private List<KCell> _cells;
  private List<KCellSerial> _serial_cells;

  /// <summary>
  /// The Cell Key
  /// </summary>
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

  /// <summary>
  /// The Key's Cells, defined as KCell items in a list-container
  /// </summary>
  public List<KCell> Cells
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
  /// The Key's Serial Cells, defined as KCellSerial items in a list-container
  /// </summary>
  public List<KCellSerial> Serial_cells
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
    public bool k;
    public bool cells;
    public bool serial_cells;
  }

  public kCells()
  {
  }

  public kCells DeepCopy()
  {
    var tmp234 = new kCells();
    if((K != null) && __isset.k)
    {
      tmp234.K = this.K.DeepCopy();
    }
    tmp234.__isset.k = this.__isset.k;
    if((Cells != null) && __isset.cells)
    {
      tmp234.Cells = this.Cells.DeepCopy();
    }
    tmp234.__isset.cells = this.__isset.cells;
    if((Serial_cells != null) && __isset.serial_cells)
    {
      tmp234.Serial_cells = this.Serial_cells.DeepCopy();
    }
    tmp234.__isset.serial_cells = this.__isset.serial_cells;
    return tmp234;
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
            if (field.Type == TType.List)
            {
              {
                TList _list235 = await iprot.ReadListBeginAsync(cancellationToken);
                K = new List<byte[]>(_list235.Count);
                for(int _i236 = 0; _i236 < _list235.Count; ++_i236)
                {
                  byte[] _elem237;
                  _elem237 = await iprot.ReadBinaryAsync(cancellationToken);
                  K.Add(_elem237);
                }
                await iprot.ReadListEndAsync(cancellationToken);
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
                TList _list238 = await iprot.ReadListBeginAsync(cancellationToken);
                Cells = new List<KCell>(_list238.Count);
                for(int _i239 = 0; _i239 < _list238.Count; ++_i239)
                {
                  KCell _elem240;
                  _elem240 = new KCell();
                  await _elem240.ReadAsync(iprot, cancellationToken);
                  Cells.Add(_elem240);
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
                TList _list241 = await iprot.ReadListBeginAsync(cancellationToken);
                Serial_cells = new List<KCellSerial>(_list241.Count);
                for(int _i242 = 0; _i242 < _list241.Count; ++_i242)
                {
                  KCellSerial _elem243;
                  _elem243 = new KCellSerial();
                  await _elem243.ReadAsync(iprot, cancellationToken);
                  Serial_cells.Add(_elem243);
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
      var struc = new TStruct("kCells");
      await oprot.WriteStructBeginAsync(struc, cancellationToken);
      var field = new TField();
      if((K != null) && __isset.k)
      {
        field.Name = "k";
        field.Type = TType.List;
        field.ID = 1;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        {
          await oprot.WriteListBeginAsync(new TList(TType.String, K.Count), cancellationToken);
          foreach (byte[] _iter244 in K)
          {
            await oprot.WriteBinaryAsync(_iter244, cancellationToken);
          }
          await oprot.WriteListEndAsync(cancellationToken);
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
          foreach (KCell _iter245 in Cells)
          {
            await _iter245.WriteAsync(oprot, cancellationToken);
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
          foreach (KCellSerial _iter246 in Serial_cells)
          {
            await _iter246.WriteAsync(oprot, cancellationToken);
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
    if (!(that is kCells other)) return false;
    if (ReferenceEquals(this, other)) return true;
    return ((__isset.k == other.__isset.k) && ((!__isset.k) || (System.Object.Equals(K, other.K))))
      && ((__isset.cells == other.__isset.cells) && ((!__isset.cells) || (TCollections.Equals(Cells, other.Cells))))
      && ((__isset.serial_cells == other.__isset.serial_cells) && ((!__isset.serial_cells) || (TCollections.Equals(Serial_cells, other.Serial_cells))));
  }

  public override int GetHashCode() {
    int hashcode = 157;
    unchecked {
      if((K != null) && __isset.k)
      {
        hashcode = (hashcode * 397) + K.GetHashCode();
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
    var sb = new StringBuilder("kCells(");
    int tmp247 = 0;
    if((K != null) && __isset.k)
    {
      if(0 < tmp247++) { sb.Append(", "); }
      sb.Append("K: ");
      K.ToString(sb);
    }
    if((Cells != null) && __isset.cells)
    {
      if(0 < tmp247++) { sb.Append(", "); }
      sb.Append("Cells: ");
      Cells.ToString(sb);
    }
    if((Serial_cells != null) && __isset.serial_cells)
    {
      if(0 < tmp247++) { sb.Append(", "); }
      sb.Append("Serial_cells: ");
      Serial_cells.ToString(sb);
    }
    sb.Append(')');
    return sb.ToString();
  }
}
