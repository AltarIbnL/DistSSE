//
// Sophos - Forward Private Searchable Encryption
// Copyright (C) 2016 Raphael Bost
//
// This file is part of Sophos.
//
// Sophos is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// Sophos is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Sophos.  If not, see <http://www.gnu.org/licenses/>.
//


#pragma once

#include "sophos_core.hpp"

#include <mutex>
#include <atomic>
#include <memory>

namespace sse {
namespace sophos {

class LargeStorageSophosClient : public SophosClient {
public:
    static std::unique_ptr<SophosClient> construct_from_directory(const std::string& dir_path);
    static std::unique_ptr<SophosClient> init_in_directory(const std::string& dir_path, uint32_t n_keywords);

    static std::unique_ptr<SophosClient> construct_from_json(const std::string& token_map_path, const std::string& keyword_indexer_path, const std::string& json_path);
    
    LargeStorageSophosClient(const std::string& token_map_path, const std::string& keyword_indexer_path, const size_t tm_setup_size);
    LargeStorageSophosClient(const std::string& token_map_path, const std::string& keyword_indexer_path, const std::string& tdp_private_key, const std::string& derivation_master_key);
    LargeStorageSophosClient(const std::string& token_map_path, const std::string& keyword_indexer_path, const std::string& tdp_private_key, const std::string& derivation_master_key, const size_t tm_setup_size);
    ~LargeStorageSophosClient();
    
    size_t keyword_count() const;
    
    SearchRequest   search_request(const std::string &keyword) const;
    SearchRequest   search_request_with_counter(const std::string &keyword, const uint32_t kw_counter) const;            
    UpdateRequest   update_request(const std::string &keyword, const index_type index);
    UpdateRequest   update_request_with_counter(const std::string &keyword, const index_type index, uint32_t &kw_counter_); // added by xiangfu
    
    
    std::ostream& db_to_json(std::ostream& out) const;
    std::ostream& print_stats(std::ostream& out) const;

    static const std::string token_map_file__;
    static const std::string keyword_counter_file__;

private:
    class JSONHandler;
    friend JSONHandler;
    
    void load_keyword_indices(const std::string &path);
    
    int64_t find_keyword_index(const std::string &kw) const;
    uint32_t get_keyword_index(const std::string &kw);
    uint32_t get_keyword_index(const std::string &kw, bool& is_new);
    uint32_t new_keyword_index(const std::string &kw);
    
    ssdmap::bucket_map< uint32_t, std::pair<search_token_type, uint32_t> > token_map_;
    std::map<std::string, uint32_t> keyword_indices_;
    
    std::ofstream keyword_indexer_stream_;
    
    std::mutex kw_index_mtx_;
    std::mutex token_map_mtx_;
    std::atomic_uint keyword_counter_;
};

}
}