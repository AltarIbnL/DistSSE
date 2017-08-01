/*
 * Created by Xiangfu Song on 10/21/2016.
 * Email: bintasong@gmail.com
 * 
 */
#ifndef DISTSSE_SERVER_H
#define DISTSSE_SERVER_H

#include <grpc++/grpc++.h>

#include "DistSSE.grpc.pb.h"

#include "DistSSE.Util.h"

#include "logger.h"


#define min(x ,y) ( x < y ? x : y)
#define max(x, y) (x < y ? y : x)

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerWriter;
using grpc::Status;


namespace DistSSE{

class DistSSEServiceImpl final : public RPC::Service {
private:	
	static rocksdb::DB* ss_db;
	// static rocksdb::DB* ss_db_read;
	static rocksdb::DB* cache_db;
    int MAX_THREADS;

	//static std::mutex ssdb_write_mtx;
	//static std::mutex cache_write_mtx;

public:
	DistSSEServiceImpl(const std::string db_path, std::string cache_path, int concurrent){

  		signal(SIGINT, abort);

		rocksdb::Options options;
    	options.create_if_missing = true;
	    Util::set_db_common_options(options);
		// options.use_fsync = true;

		rocksdb::Status s1 = rocksdb::DB::Open(options, db_path, &ss_db);
		// rocksdb::Status s2 = rocksdb::DB::OpenForReadOnly(options, db_path, &ss_db_read);

		// set options for merge operation
		rocksdb::Options simple_options;
		simple_options.create_if_missing = true;
		simple_options.merge_operator.reset(new rocksdb::StringAppendOperator() );
		simple_options.use_fsync = true;

		rocksdb::Status s3 = rocksdb::DB::Open(simple_options, cache_path, &cache_db);

		assert(s1.ok());
		// assert(s2.ok());
		assert(s3.ok());

		MAX_THREADS = concurrent; //std::thread::hardware_concurrency();
	}

	static void abort( int signum )
	{
		delete ss_db;
		delete cache_db; 
		logger::log(logger::INFO)<< "abort: "<< signum <<std::endl;
	   	exit(signum);
	}

	static int store(rocksdb::DB* &db, const std::string l, const std::string e){
		rocksdb::Status s; 	
		rocksdb::WriteOptions write_option = rocksdb::WriteOptions();
		// write_option.sync = true;
		// write_option.disableWAL = false;
		{
			//std::lock_guard<std::mutex> lock(ssdb_write_mtx);		
			s = db->Put(write_option, l, e);
		}

		assert(s.ok());
		if (s.ok())	return 0;
		else return -1;
	}

	static std::string get(rocksdb::DB* &db, const std::string l){
		std::string tmp;
		rocksdb::Status s;
	
	//redo:
		{
			//std::lock_guard<std::mutex> lock(ssdb_write_mtx);
			s = db->Get(rocksdb::ReadOptions(), l, &tmp);
		}

		if (!s.ok())	std::cerr << "in get() " << s.ToString()<<", tmp: "<< tmp << std::endl;
		
 	 return tmp;
	}

	static int merge(rocksdb::DB* &db, const std::string l, const std::string append_str) {

		int status = -1;
		
		try{

			rocksdb::WriteOptions write_option = rocksdb::WriteOptions();
			write_option.sync = true;
			//write_option.disableWAL = true;
			rocksdb::Status s;
			{
				//std::lock_guard<std::mutex> lock(cache_write_mtx);
				s = db->Delete(write_option, l);
				s = db->Put(write_option, l, append_str);
			}

			if (s.ok())	status = 0;

		}catch(std::exception &e) {
			std::cerr << "in append_cache() " << e.what()<< std::endl;
			exit(1);
		}

		return status;
	}

	static int delete_entry(rocksdb::DB* &db, const std::string l) {
		int status = -1;
		
		try{

			rocksdb::WriteOptions write_option = rocksdb::WriteOptions();
			//write_option.sync = true;
			// write_option.disableWAL = true;
			rocksdb::Status s;
			{
				//std::lock_guard<std::mutex> lock(ssdb_write_mtx);				
				s = db->Delete(write_option, l);
			}
			if (s.ok())	status = 0;

		}catch(std::exception &e) {
			std::cerr << "in delete_entry() " << e.what()<< std::endl;
			exit(1);
		}

		return status;
	}

	static void search_log(std::string kw, double search_time, int result_size) {
		// std::ofstream out( "search.slog", std::ios::out|std::ios::app);
		byte k_s[17] = "0123456789abcdef";
		byte iv_s[17] = "0123456789abcdef";

		std::string keyword = Util::dec_token(k_s, AES128_KEY_LEN, iv_s, kw);
			
		std::string word = keyword == "" ? "cached" : keyword;	
		
		std::cout <<  word + "\t" + std::to_string(result_size)+ "\t" + std::to_string(search_time) + "\t" + std::to_string(search_time/result_size) << std::endl;
	}

	static void parse (std::string str, std::string& op, std::string& ind) {
		op = str.substr(0, 1);		
		ind = str.substr(1, 7); // TODO
	}

	static void search_task(int threadID, std::string kw, int begin, int end, std::set<std::string>* result_set) {
		std::string ind, op;
		std::string l, e, value;

		bool flag = false;
		for(int i = begin + 1; i <= end; i++) {

			l = Util::H1(kw + std::to_string(i));
			// logger::log(logger::INFO) << "server.search(), l:" << l << ", kw: " << kw <<std::endl;
			e = get(ss_db, l);
			if (e.compare("") == 0) {// unknow reason for read losing TODO
				logger::log(logger::ERROR) << "ERROR in search, null str found: " << l << l.length() <<std::endl;
				//continue;
			}
			
			value = Util::Xor( e, Util::H2(kw + std::to_string(i)) );

			parse(value, op, ind);
			
			// logger::log(logger::INFO) << "value: " << value <<std::endl;
			/*if(op == "1")*/ result_set->insert(value); // TODO
			// else result_set->erase(ind);
			// if (i % 1000 == 0) logger::log(logger::INFO) << "Thread ID: " << threadID << ", searched: " << i << "\n" <<std::flush;
			
			//  int s = delete_entry(ss_db, l);	
			//  assert(s == 0);
		}
	}


	static void search_single(std::string kw, int begin, int end, std::set<std::string>& ID, std::string& merge_string) {
		std::string ind, op;
		std::string l, e, value;

		for(int i = begin + 1; i <= end; i++) {

			l = Util::H1(kw + std::to_string(i));
			// logger::log(logger::INFO) << "server.search(), l:" << l << ", kw: " << kw <<std::endl;
			
			bool flag = false;

			// redo:
			e = get(ss_db, l);

			if (e.compare("") == 0) {
				logger::log(logger::ERROR) << "ERROR in search, null str found, i= "<< i<<", begin: " << begin<< ", end: " << end <<std::endl;
				
				flag = true;
				// goto redo;
				// continue;
			}
			// if(flag) exit(-1);
			
			value = Util::Xor(e, Util::H2(kw + std::to_string(i)) );

			parse(value, op, ind);
			
			/*if(op == "1")*/  ID.insert(value);

			merge_string += Util::str2hex(value) + "|";
		}
	}

	static void merge( std::set<std::string>* searchResult, int counter, std::set<std::string> &merge_result, std::string& merge_string){
		for (int i = 0; i < counter; i++) {
			for (auto& t : searchResult[i]) {
				merge_result.insert(t);
				merge_string += Util::str2hex(t) + "|";
			}
		}
	}

	void search(std::string kw, std::string tw, int uc, std::set<std::string>& ID){
	
		std::vector<std::string> op_ind;

		std::string ind, op;
		std::string l, e, value;
		std::string cache_ind;

		int counter = 0;

		struct timeval t1, t2;

		double search_time;

		try{
			
			gettimeofday(&t1, NULL);

			std::string merge_string = get(cache_db, tw);
			//Util::split(merge_string, '|', ID); // get all cached inds					
			gettimeofday(&t2, NULL);

			//std::string merge_string = cache_ind;
			
			if(kw != "") {

				if (uc < 1000) {

					search_single( kw, 0, uc, ID, merge_string );
					// std::cout<< "ID.size(): " << ID.size() <<std::endl;
	
				}else {
					
					int thread_num = min( uc / 1000, MAX_THREADS);
					
					int step = uc / thread_num + 1;
					
					std::set<std::string> result_set[MAX_THREADS]; // result ID lists for storage nodes

					std::vector<std::thread> threads;
				
					for (int i = 0; i < thread_num; i++) {
						int left = i * step, right = min((i + 1) * step, uc);
						if(left < right) threads.push_back( std::thread(search_task, i,  kw, left, right, &(result_set[i])) );
					}

					// join theads
					for (auto& t : threads) {
						t.join();
					}

					merge(result_set, MAX_THREADS, ID, merge_string);
				}

				// merge to cache can be done by a seperate thread backgroud, the result can be returned before.
				// so we don't count the time...
			}
			
			gettimeofday(&t2, NULL);		

			if (merge_string != "") {
				int s = merge(cache_db, tw, merge_string);
				assert(s == 0);
			}
			
			search_time =  ((t2.tv_sec - t1.tv_sec) * 1000000.0 + t2.tv_usec - t1.tv_usec) / 1000.0 ;

			search_log(kw, search_time, ID.size());


		}catch(const std::exception& e)
		{
			std::cerr << "in Search() " << e.what()<< std::endl;
			exit(1);
		}
	}

// server RPC
	// search() 实现搜索操作
	Status search(ServerContext* context, const SearchRequestMessage* request,
                  ServerWriter<SearchReply>* writer) {

		std::string kw = request->kw();
		std::string tw = request->tw();	
		int uc = request->uc();
		
		struct timeval t1, t2;

		// TODO 读取数据库之前要加锁，读取之后要解锁
		
		std::set<std::string> ID;

		// logger::log(logger::INFO) << "searching... " <<std::endl;

		// gettimeofday(&t1, NULL);
		search(kw, tw, uc, ID);
		// gettimeofday(&t2, NULL);

  		// logger::log(logger::INFO) <<"ID.size():"<< ID.size() <<" ,search time: "<< ((t2.tv_sec - t1.tv_sec) * 1000000.0 + t2.tv_usec - t1.tv_usec) /1000.0/ID.size()<<" ms" <<std::endl;
		// TODO 读取之后需要解锁

		SearchReply reply;
		
		for(int i = 0; i < ID.size(); i++){
			reply.set_ind(std::to_string(i));
			writer->Write(reply);
		}

		// logger::log(logger::INFO) << "search done." <<std::endl;

	    return Status::OK;
  	}
	
	// update()实现单次更新操作
	Status update(ServerContext* context, const UpdateRequestMessage* request, ExecuteStatus* response) {
		std::string l = request->l();
		std::string e = request->e();
		std::cout<<"in update(), counter:  "<<request->counter()<<std::endl;
		// TODO 更新数据库之前要加锁

		int status = store(ss_db, l, e);
		// TODO 更新之后需要解锁
		assert(status == 0);

		if(status != 0) {
			response->set_status(false);
			return Status::CANCELLED;
		}
		response->set_status(true);
		return Status::OK;
	}
	
	// batch_update()实现批量更新操作
	Status batch_update(ServerContext* context, ServerReader< UpdateRequestMessage >* reader, ExecuteStatus* response) {
		std::string l;
		std::string e;
		// TODO 读取数据库之前要加锁，读取之后要解锁
		UpdateRequestMessage request;
		while (reader->Read(&request)) {
			l = request.l();
			e = request.e();
			// std::cout<<"in update(), counter:  "<<request.counter()<<std::endl;
			store(ss_db, l, e);
		  //  assert(status == 0);
		}
		// TODO 读取之后需要解锁

		response->set_status(true);
		return Status::OK;
	}

	// batch_cache(), only used for expriment simulation
	Status batch_cache(ServerContext* context, ServerReader< CacheRequestMessage >* reader, ExecuteStatus* response) {
		std::string tw;
		std::string inds;
		// TODO 读取数据库之前要加锁，读取之后要解锁
		CacheRequestMessage request;
		while (reader->Read(&request)){
			tw = request.tw();
			inds = request.inds();
			int s = merge(cache_db, tw, inds);
			assert(s == 0);
		}

		response->set_status(true);
		return Status::OK;
	}
};

}// namespace DistSSE

// static member must declare out of main function !!!
rocksdb::DB* DistSSE::DistSSEServiceImpl::ss_db;
// rocksdb::DB* DistSSE::DistSSEServiceImpl::ss_db_read;
rocksdb::DB* DistSSE::DistSSEServiceImpl::cache_db;

//std::mutex DistSSE::DistSSEServiceImpl::ssdb_write_mtx;
//std::mutex DistSSE::DistSSEServiceImpl::cache_write_mtx;

void RunServer(std::string db_path, std::string cache_path, int concurrent) {


  std::string server_address("0.0.0.0:50051");
  DistSSE::DistSSEServiceImpl service(db_path, cache_path, concurrent);
  
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  DistSSE::logger::log(DistSSE::logger::INFO) << "Server listening on " << server_address << std::endl;

  server->Wait();
}

#endif // DISTSSE_SERVER_H