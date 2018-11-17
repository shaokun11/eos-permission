#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] learnper2 : public contract {
  public:
    using contract::contract;
    learnper2(eosio::name reciever,eosio::name code,eosio::datastream<const char*> ds )
                              :contract(reciever,code,ds),
                              _students(reciever,code.value){};

    [[eosio::action]]
    void add(name user, const std::string msg) {
      require_auth(user); // 权限额验证 
      auto itr = _students.find(user.value);
      if (itr == _students.end()){
        _students.emplace(get_self(),[&](auto& row){
          row.user = user;
          row.msg = msg;
        });
      } else {
        _students.modify(itr, get_self(), [&](auto& row){
          row.msg = msg;
        });
      }
    };


  private:
  struct [[eosio::table]] student 
  {
    name user;
    std::string msg = "hello world";
    uint64_t primary_key() const {return user.value;};

  };
  typedef eosio::multi_index<"student"_n, student> student_index;
  student_index _students; 
};
EOSIO_DISPATCH( learnper2, (add))