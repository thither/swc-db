/**
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 *
 * This file and its generated files are licensed under the Apache License,
 * Version 2.0 (the "License"); You may not use this file and its generated
 * files except in compliance with the License. You may obtain a copy of the
 * License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/**
 * namespace for target languages
 */

namespace cpp   SWC.Thrift
namespace py    SWC.client.thrift.gen2
/*
namespace java  org.swcdb.thriftgen
namespace perl  SWC.ThriftGen2
namespace php   SWC # ditto
namespace rb    SWC.ThriftGen
*/


exception Exception {
  1: i32 code
  2: string message
}

enum Flag {
  NONE            = 0,
  INSERT          = 1,
  DELETE          = 2,
  DELETE_VERSION  = 3
}

struct Cell {
  1: list<binary>     k
  2: i64              ts
  3: optional binary  v
}
typedef list<Cell> Cells


service Service {
  Cells select_sql(1:string sql) throws (1:Exception e),
}
