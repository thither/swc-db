
/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swcdb_db_client_Query_Profiling_h
#define swcdb_db_client_Query_Profiling_h



namespace SWC { namespace client {


//! The SWC-DB Query C++ namespace 'SWC::client::Query'
namespace Query {



//! The SWC-DB Result C++ namespace 'SWC::client::Query::Result'
namespace Result { }



using ReqBase = Comm::client::ConnQueue::ReqBase;


struct Profiling {
  const int64_t          ts_start;
  Core::Atomic<int64_t>  ts_finish;

  Profiling() noexcept : ts_start(Time::now_ns()), ts_finish(ts_start) { }

  struct Component {

    Core::Atomic<uint64_t> count;
    Core::Atomic<uint64_t> time;
    Core::Atomic<uint64_t> error;

    Component() noexcept : count(0), time(0), error(0) { }

    struct Start {
      Component&    _m;
      const int64_t ts;

      Start(Component& m) noexcept
            : _m(m), ts(Time::now_ns()) {
      }

      void add(bool err) const noexcept {
        _m.add(ts, err);
      }
    };

    Start start() noexcept {
      return Start(*this);
    }

    void add(uint64_t ts, bool err) noexcept {
      count.fetch_add(1);
      time.fetch_add(Time::now_ns() - ts);
      if(err)
        error.fetch_add(1);
    }

    void print(std::ostream& out) const {
      out << "(count=" << count.load()
          << " time=" << time.load() << "ns errors=" << error.load() << ')';
    }

    void display(std::ostream& out) const {
      out << time.load() << "ns" << '/'
          << count.load() << '(' << error.load() << ")\n";
    }
  };

  Component _mngr_locate;
  Component _mngr_res;
  Component _rgr_locate_master;
  Component _rgr_locate_meta;
  Component _rgr_data;

  void finished() noexcept {
    ts_finish.store(Time::now_ns());
  }

  Component::Start mngr_locate() noexcept {
    return Component::Start(_mngr_locate);
  }

  Component::Start mngr_res() noexcept {
    return Component::Start(_mngr_res);
  }

  Component::Start rgr_locate(DB::Types::Range type) noexcept {
    switch(type) {
      case DB::Types::Range::MASTER:
        return Component::Start(_rgr_locate_master);
      default:
        return Component::Start(_rgr_locate_meta);
    }
  }

  Component::Start rgr_data() noexcept {
    return Component::Start(_rgr_data);
  }

  void display(std::ostream& out) const {
    _mngr_locate.display(
      out << " Mngr Locate:            ");
    _mngr_res.display(
      out << " Mngr Resolve:           ");
    _rgr_locate_master.display(
      out << " Rgr Locate Master:      ");
    _rgr_locate_meta.display(
      out << " Rgr Locate Meta:        ");
    _rgr_data.display(
      out << " Rgr Data:               ");
    out  << std::flush;
  }

  void print(std::ostream& out) const {
    out << "Profile(took=" << (ts_finish - ts_start) << "ns";
    _mngr_locate.print(out << " mngr[locate");
    _mngr_res.print(out << " res");
    _rgr_locate_master.print(out << "] rgr[locate-master");
    _rgr_locate_meta.print(out << " locate-meta");
    _rgr_data.print(out << " data");
    out << "])";
  }

  std::string to_string() const {
    std::stringstream ss;
    print(ss);
    return ss.str();
  }

};


}}}

#endif // swcdb_db_client_Query_Profiling_h