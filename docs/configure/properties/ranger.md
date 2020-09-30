---
title: Ranger Configuration
---



# The SWC-DB Ranger - Configuration Properties



* ### swc.rgr.cfg.dyn
```STRINGS```
The Dynamic cfg-file to use. Rangers will check & reload this cfg-file by swc.cfg.dyn.period \
_default_ **```swc.rgr.cfg.dyn=```**

* ### swc.rgr.reactors
```INT32```
The number of communication reactor to initialize an io-context for a given acceptor-fd. \
_default_ **```swc.rgr.reactors=8```**

* ### swc.rgr.workers
```INT32```
The number of workers a comm-reactor initalizes. \
_default_ **```swc.rgr.workers=32```**

* ### swc.rgr.handlers
```INT32```
The number of Application handlers. \
_default_ **```swc.rgr.handlers=8```**


* ### swc.rgr.maintenance.handlers
```INT32```
The number of Maintenance handlers. \
_default_ **```swc.rgr.maintenance.handlers=2```**



***

 > **_The configuration properties applicable for dynamic reloading_**


* ### swc.rgr.ram.allowed.percent
```G_INT32```
The Memory RSS % allowed without freeing/releasing. \
_default_ **```swc.rgr.ram.allowed.percent=33```**

* ### swc.rgr.ram.reserved.percent
```G_INT32```
The Memory Total % reserved, threshold of low-memory enter state. \
_default_ **```swc.rgr.ram.reserved.percent=33```**

* ### swc.rgr.ram.release.rate
```G_INT32```
The Memory release-rate (malloc dependable). \
_default_ **```swc.rgr.ram.release.rate=100```**

* ### swc.rgr.id.validation.interval
```G_INT32```
The Validation of Ranger-ID against Manager(RAGERS ROLE). \
_default_ **```swc.rgr.id.validation.interval=120000```**

* ### swc.rgr.compaction.check.interval
```G_INT32```
The Interval in milliseconds for Compaction. \
_default_ **```swc.rgr.compaction.check.interval=300000```**

* ### swc.rgr.compaction.read.ahead
```G_INT8```
The Allowed read-ahead scans per Range compaction. \
_default_ **```swc.rgr.compaction.read.ahead=5```**

* ### swc.rgr.compaction.range.max
```G_INT8```
The Max Allowed Ranges at a time for compaction. \
_default_ **```swc.rgr.compaction.range.max=2```**


  > ***
  > _Default Schema Values_

* ### swc.rgr.Range.CellStore.count.max
```G_INT8```
The Schema default cellstore-max in range before range-split. \
_default_ **```swc.rgr.Range.CellStore.count.max=10```**


* ### swc.rgr.Range.CellStore.size.max
```G_INT32```
The Schema default cellstore-size. \
_default_ **```swc.rgr.Range.CellStore.size.max=1GB```**

* ### swc.rgr.Range.CellStore.replication
```G_INT8```
The Schema default cellstore-replication (fs-dependent). \
_default_ **```swc.rgr.Range.CellStore.replication=3```**

* ### swc.rgr.Range.block.size
```G_INT32```
The Schema default block-size. \
_default_ **```swc.rgr.Range.block.size=64MB```**

* ### swc.rgr.Range.block.cells
```G_INT32```
The Schema default block-cells. \
_default_ **```swc.rgr.Range.block.cells=100000```**

* ### swc.rgr.Range.block.encoding
```G_ENUM```
The Schema default block-encoding NONE/ZSTD/SNAPPY/ZLIB. \
_default_ **```swc.rgr.Range.block.encoding=ZSTD```**

* ### swc.rgr.Range.CommitLog.rollout.ratio
```G_INT8```
The Schema default CommitLog new fragment Rollout Block Ratio. \
_default_ **```swc.rgr.Range.CommitLog.rollout.ratio=3```**

* ### swc.rgr.Range.compaction.percent
```G_INT8```
The Schema default compact-percent threshold. \
_default_ **```swc.rgr.Range.compaction.percent=33```**



***

 > _**extended/updated/version information available with '--help' and '--help-config' arg**_

```
./Ranger --help;
```

```
./Ranger --help-config;
```