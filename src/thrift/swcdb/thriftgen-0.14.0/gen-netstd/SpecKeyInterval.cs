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
/// The Key Interval Specifications
/// </summary>
public partial class SpecKeyInterval : TBase
{
  private List<SpecFraction> _start;
  private List<SpecFraction> _finish;

  /// <summary>
  /// The Key Start Spec, the start of cells-interval key match
  /// </summary>
  public List<SpecFraction> Start
  {
    get
    {
      return _start;
    }
    set
    {
      __isset.start = true;
      this._start = value;
    }
  }

  /// <summary>
  /// The Key Finish Spec, the finish of cells-interval key match
  /// </summary>
  public List<SpecFraction> Finish
  {
    get
    {
      return _finish;
    }
    set
    {
      __isset.finish = true;
      this._finish = value;
    }
  }


  public Isset __isset;
  public struct Isset
  {
    public bool start;
    public bool finish;
  }

  public SpecKeyInterval()
  {
  }

  public SpecKeyInterval DeepCopy()
  {
    var tmp26 = new SpecKeyInterval();
    if((Start != null) && __isset.start)
    {
      tmp26.Start = this.Start.DeepCopy();
    }
    tmp26.__isset.start = this.__isset.start;
    if((Finish != null) && __isset.finish)
    {
      tmp26.Finish = this.Finish.DeepCopy();
    }
    tmp26.__isset.finish = this.__isset.finish;
    return tmp26;
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
                TList _list27 = await iprot.ReadListBeginAsync(cancellationToken);
                Start = new List<SpecFraction>(_list27.Count);
                for(int _i28 = 0; _i28 < _list27.Count; ++_i28)
                {
                  SpecFraction _elem29;
                  _elem29 = new SpecFraction();
                  await _elem29.ReadAsync(iprot, cancellationToken);
                  Start.Add(_elem29);
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
                TList _list30 = await iprot.ReadListBeginAsync(cancellationToken);
                Finish = new List<SpecFraction>(_list30.Count);
                for(int _i31 = 0; _i31 < _list30.Count; ++_i31)
                {
                  SpecFraction _elem32;
                  _elem32 = new SpecFraction();
                  await _elem32.ReadAsync(iprot, cancellationToken);
                  Finish.Add(_elem32);
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
      var struc = new TStruct("SpecKeyInterval");
      await oprot.WriteStructBeginAsync(struc, cancellationToken);
      var field = new TField();
      if((Start != null) && __isset.start)
      {
        field.Name = "start";
        field.Type = TType.List;
        field.ID = 1;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        {
          await oprot.WriteListBeginAsync(new TList(TType.Struct, Start.Count), cancellationToken);
          foreach (SpecFraction _iter33 in Start)
          {
            await _iter33.WriteAsync(oprot, cancellationToken);
          }
          await oprot.WriteListEndAsync(cancellationToken);
        }
        await oprot.WriteFieldEndAsync(cancellationToken);
      }
      if((Finish != null) && __isset.finish)
      {
        field.Name = "finish";
        field.Type = TType.List;
        field.ID = 2;
        await oprot.WriteFieldBeginAsync(field, cancellationToken);
        {
          await oprot.WriteListBeginAsync(new TList(TType.Struct, Finish.Count), cancellationToken);
          foreach (SpecFraction _iter34 in Finish)
          {
            await _iter34.WriteAsync(oprot, cancellationToken);
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
    if (!(that is SpecKeyInterval other)) return false;
    if (ReferenceEquals(this, other)) return true;
    return ((__isset.start == other.__isset.start) && ((!__isset.start) || (System.Object.Equals(Start, other.Start))))
      && ((__isset.finish == other.__isset.finish) && ((!__isset.finish) || (System.Object.Equals(Finish, other.Finish))));
  }

  public override int GetHashCode() {
    int hashcode = 157;
    unchecked {
      if((Start != null) && __isset.start)
      {
        hashcode = (hashcode * 397) + Start.GetHashCode();
      }
      if((Finish != null) && __isset.finish)
      {
        hashcode = (hashcode * 397) + Finish.GetHashCode();
      }
    }
    return hashcode;
  }

  public override string ToString()
  {
    var sb = new StringBuilder("SpecKeyInterval(");
    int tmp35 = 0;
    if((Start != null) && __isset.start)
    {
      if(0 < tmp35++) { sb.Append(", "); }
      sb.Append("Start: ");
      Start.ToString(sb);
    }
    if((Finish != null) && __isset.finish)
    {
      if(0 < tmp35++) { sb.Append(", "); }
      sb.Append("Finish: ");
      Finish.ToString(sb);
    }
    sb.Append(')');
    return sb.ToString();
  }
}

