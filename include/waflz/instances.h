//! ----------------------------------------------------------------------------
//! Copyright Verizon.
//!
//! \file:    TODO
//! \details: TODO
//!
//! Licensed under the terms of the Apache 2.0 open source license.
//! Please refer to the LICENSE file in the project root for the terms.
//! ----------------------------------------------------------------------------
#ifndef _INSTANCES_H_
#define _INSTANCES_H_
//! ----------------------------------------------------------------------------
//! includes
//! ----------------------------------------------------------------------------
#include "waflz/def.h"
#include "waflz/city.h"
#include "waflz/rqst_ctx.h"
#include <pthread.h>
#include <string>
#include <vector>
#if defined(__APPLE__) || defined(__darwin__)
    #include <unordered_map>
#else
    #include <tr1/unordered_map>
#endif
namespace waflz_pb {
        class event;
        class enforcement;
}
namespace ns_waflz
{
//! ----------------------------------------------------------------------------
//! fwd decl's
//! ----------------------------------------------------------------------------
class instance;
class event;
class profile;
class geoip2_mmdb;
class engine;
class rqst_ctx;
//! ----------------------------------------------------------------------------
//! instances
//! ----------------------------------------------------------------------------
class instances
{
public:
        // -------------------------------------------------
        // Public methods
        // -------------------------------------------------
        instances(engine &a_engine,
                  bool a_enable_locking = false);
        ~instances();
        int32_t load_file(instance **ao_instance,
                          const char *a_file_path,
                          uint32_t a_file_path_len,
                          bool a_update = false);
        int32_t load(instance **ao_instance,
                     const char *a_buf,
                     uint32_t a_buf_len,
                     bool a_update = false);
        int32_t load_dir(const char *a_dir_path,
                         uint32_t a_dir_path_len,
                         bool a_update = false);
        int32_t process(waflz_pb::enforcement **ao_enf,
                        waflz_pb::event **ao_audit_event,
                        waflz_pb::event **ao_prod_event,
                        void *a_ctx,
                        const std::string &a_id,
                        part_mk_t a_part_mk,
                        const rqst_ctx_callbacks *a_callbacks,
                        rqst_ctx **ao_rqst_ctx);
        void set_locking(bool a_enable_locking) { m_enable_locking = a_enable_locking; }
        void get_first_id(std::string &ao_id);
        void get_rand_id(std::string &ao_id);
        bool id_exists(bool& ao_audit, bool &ao_prod, const std::string& a_id);
        const char *get_err_msg(void) { return m_err_msg; }
private:
        // -------------------------------------------------
        // private types
        // -------------------------------------------------
        typedef std::vector <std::string> id_vector_t;
        // -------------------------------------------------
        // str hash
        // -------------------------------------------------
        struct str_hash
        {
                inline std::size_t operator()(const std::string& a_key) const
                {
                        return CityHash64(a_key.c_str(), a_key.length());
                }
        };
#if defined(__APPLE__) || defined(__darwin__)
        typedef std::unordered_map<std::string, instance*, str_hash> id_instance_map_t;
#else
        typedef std::tr1::unordered_map<std::string, instance*, str_hash> id_instance_map_t;
#endif
        // -------------------------------------------------
        // Private methods
        // -------------------------------------------------
        // Disallow copy/assign
        instances(const instances &);
        instances& operator=(const instances &);
        int32_t load(instance **ao_instance, void *a_js, bool a_update = false);
        instance *get_instance(const std::string &a_id);
        // -------------------------------------------------
        // Private members
        // -------------------------------------------------
        char m_err_msg[WAFLZ_ERR_LEN];
        engine &m_engine;
        id_instance_map_t m_id_instance_map;
        pthread_mutex_t m_mutex;
        bool m_enable_locking;
};
}
#endif
