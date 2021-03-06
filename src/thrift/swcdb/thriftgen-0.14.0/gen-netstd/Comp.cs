/**
 * Autogenerated by Thrift Compiler (0.14.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

#pragma warning disable IDE0079  // remove unnecessary pragmas
#pragma warning disable IDE1006  // parts of the code use IDL spelling

/// <summary>
/// The available logical Comparators, plus extended logic options applied with 'v' for VOLUME
/// </summary>
public enum Comp
{
  /// <summary>
  /// [         ]  :   none               (no comparison applied)
  /// </summary>
  NONE = 0,
  /// <summary>
  /// [  =^     ]  :   -pf [prefix]       (starts-with)
  /// </summary>
  PF = 1,
  /// <summary>
  /// [ &gt;    ]  :   -gt                (greater-than)
  /// </summary>
  GT = 2,
  /// <summary>
  /// [ &gt;=   ]  :   -ge                (greater-equal)
  /// </summary>
  GE = 3,
  /// <summary>
  /// [  =      ]  :   -eq                (equal)
  /// </summary>
  EQ = 4,
  /// <summary>
  /// [ &lt;=   ]  :   -le                (lower-equal)
  /// </summary>
  LE = 5,
  /// <summary>
  /// [ &lt;    ]  :   -lt                (lower-than)
  /// </summary>
  LT = 6,
  /// <summary>
  /// [  !=     ]  :   -ne                (not-equal)
  /// </summary>
  NE = 7,
  /// <summary>
  /// [  re     ]  :   -re [r,regexp]     (regular-expression)
  /// </summary>
  RE = 8,
  /// <summary>
  /// [ v&gt;   ]  :   -vgt               (vol greater-than)
  /// </summary>
  VGT = 9,
  /// <summary>
  /// [ v&gt;=  ]  :   -vge               (vol greater-equal)
  /// </summary>
  VGE = 10,
  /// <summary>
  /// [ v&lt;=  ]  :   -vle               (vol lower-equal)
  /// </summary>
  VLE = 11,
  /// <summary>
  /// [ v&lt;   ]  :   -vlt               (vol lower-than)
  /// </summary>
  VLT = 12,
  /// <summary>
  /// [ %&gt;   ]  :   -subset [sbs]      (subset)
  /// </summary>
  SBS = 13,
  /// <summary>
  /// [ &lt;%   ]  :   -supset [sps]      (superset)
  /// </summary>
  SPS = 14,
  /// <summary>
  /// [ ~&gt;   ]  :   -posubset [posbs]  (eq/part ordered subset)
  /// </summary>
  POSBS = 15,
  /// <summary>
  /// [ &lt;~   ]  :   -posupset [posps]  (eq/part ordered superset)
  /// </summary>
  POSPS = 16,
  /// <summary>
  /// [ -&gt;   ]  :   -fosubset [fosbs]  (eq/full ordered subset)
  /// </summary>
  FOSBS = 17,
  /// <summary>
  /// [ &lt;-   ]  :   -fosupset [fosps]  (eq/full ordered superset)
  /// </summary>
  FOSPS = 18,
}
