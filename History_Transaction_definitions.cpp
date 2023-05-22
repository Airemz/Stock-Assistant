#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

// Test: g++ -o main History_Transaction_definitions.cpp project4.cpp main.cpp -std=c++11

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//

Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,  
        unsigned int month_date,  unsigned year_date, 
        bool buy_sell_trans,  unsigned int number_shares,  
        double trans_amount ){
  symbol = ticker_symbol;
  day = day_date;  
  month = month_date;  
  year = year_date;  
  shares = number_shares;  
  amount = trans_amount;

  if ((buy_sell_trans) == true){
    trans_type = "Buy";
  }

  else {
    trans_type = "Sell";
  }

  p_next = nullptr;
  trans_id = assigned_trans_id;
  assigned_trans_id++;
}




// Destructor
// TASK 1
//

Transaction::~Transaction(){
}

// Overloaded < operator.
// TASK 2
//

bool Transaction::operator<( Transaction const &other ){
  //Compare years, month, days if left is less than right
  if (this->year < other.year) {
    return true;
  }

  else if((this->year == other.year) && (this->month < other.month)){
    return true;
  }

  else if((this->year == other.year) && (this->month == other.month) && (this -> day < other.day)){
    return true;
  }

  return false;
} 
   
   

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
// DUE Friday:constructor, deconstructor, read, insert, print


// Constructor
// TASK 3
//

History::History(){
  p_head = nullptr;
}

// Destructor
// TASK 3
//

History::~History(){
  while (p_head != nullptr){
    //pop front
    Transaction *new_node =  p_head;
    p_head = p_head -> get_next();
    delete new_node;
    new_node = nullptr;
  }
}


// read_history(...): Read the transaction history from file.
// TASK 4
//

void History::read_history(){
  ece150::open_file();
  
  //Iterrate until next entry is false. Undefined operation so dont call
  while ((ece150::next_trans_entry()) == true){
    Transaction *new_transaction = new Transaction{ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(),  ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount()};
    
    History::insert(new_transaction);
  }
  ece150::close_file();
}

// insert(...): Insert transaction into linked list.
// TASK 5
//

void History::insert(Transaction *p_new_trans){
  Transaction *next_node = p_head;

  //If list empty add to linked list
  if (p_head == nullptr){
    p_head = p_new_trans;
  } else{
    //If next node is empty, dont change node to next
    while ((next_node -> get_next()) != nullptr){
      next_node = next_node -> get_next();
    }
    //Inserting to list
    next_node -> set_next(p_new_trans);
  }
}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//

void History::sort_by_date(){
  //Loop through list
  //Use insertion sort

  //Head of sorted linked list
  Transaction *sorted = nullptr;
  Transaction *remove_node = nullptr;

  //Loop through linked list until end
  while (p_head != nullptr){

    //Remove first node
    remove_node =  p_head;
    p_head = p_head -> get_next();
    remove_node -> set_next(nullptr);

    //Sorted list is empty, set to remove node and reset remove node
    if (sorted == nullptr){
      sorted = remove_node;
      // sorted -> set_next(nullptr);
      remove_node = nullptr;
      std::cout << "first node set" << std::endl;
    }

    //If remove node is less than sorted list head (only head is in list)
    else if (*remove_node < *sorted){
      remove_node -> set_next(sorted);
      sorted = remove_node;
      remove_node = nullptr;
      std::cout << "second node set" << std::endl;
    }

    //If remove node is greater than sorted list head (only head is in list)
    else if ((sorted -> get_next()) == nullptr){
      sorted -> set_next(remove_node);
      remove_node -> set_next(nullptr);
      remove_node = nullptr;
      std::cout << "second(last) node set" << std::endl;
    }


    //remove_node needs to be placed in middle of sorted list or at end 
    else {
      Transaction *looping_node = sorted;

      //loop through sorted list until at end or until reaching a node greater than remove_node
      while(((looping_node -> get_next()) != nullptr) && (*(looping_node -> get_next()) < *remove_node)){
        looping_node = looping_node -> get_next(); 
      }

      //Place remove_node after looping_node and before the node after looping node (oof)
      remove_node -> set_next(looping_node -> get_next());
      looping_node -> set_next(remove_node);

      //reseting
      remove_node = nullptr;
      looping_node = nullptr;
      std::cout << "rest node set" << std::endl;
    }
  }

  //After list is complete, set p_head to head of sorted list
  p_head = sorted;
}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//


void History::update_acb_cgl(){
  double acb = 0;
  double acb_per_share = 0;
  unsigned int share_balance = 0;
  double cgl = 0;

  //looping through linked list
  Transaction *looping_node = p_head;
  while(looping_node != nullptr){

    //Buy
    if ((looping_node -> get_trans_type()) == true){

      //adding total amount to acb and set to the node
      acb += looping_node -> get_amount();
      looping_node -> set_acb(acb);

      //adding total shares to share balance and set to the node
      share_balance += looping_node -> get_shares();
      looping_node -> set_share_balance(share_balance);

      //Divide acb by share balance and set to node
      acb_per_share = acb / share_balance;
      looping_node -> set_acb_per_share(acb_per_share);

      //cgl is unaffected when buy, set to 0 to avoid random numbers
      cgl = 0;
      looping_node -> set_cgl(cgl);

    }

    //Sell
    else{

      //previous acb - shares sold * previous acb/share
      acb -= (looping_node -> get_shares() * acb_per_share);
      looping_node -> set_acb(acb);

      //previous share balance - shares sold
      share_balance -= looping_node -> get_shares();
      looping_node -> set_share_balance(share_balance);

      //Divide acb by share balance and set to node
      acb_per_share = acb / share_balance;
      looping_node -> set_acb_per_share(acb_per_share);

      //previous transaction - (shares sold * acb/share)
      cgl = looping_node -> get_amount() - (looping_node -> get_shares() * acb_per_share);
      looping_node -> set_cgl(cgl);

    }

    looping_node = looping_node -> get_next();

  }
}


// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8

double History::compute_cgl(unsigned int year) {
  double cgl = 0;

  Transaction *looping_node = p_head;
  while(looping_node != nullptr){

    //Sum all cgls in the specfied year
    if ((looping_node -> get_year()) == year){
      cgl += looping_node -> get_cgl();
    }

    looping_node = looping_node -> get_next();

  }
  
  return cgl;
}


// print() Print the transaction history.
//TASK 9
//

void History::print(){
  std::cout << "========== BEGIN TRANSACTION HISTORY ============"
  << std::endl;
  Transaction* next_node = p_head;

  //Printing all nodes until end(nullptr)
  while (next_node != nullptr){
    next_node -> print();
    next_node = next_node -> get_next();
  }

  std::cout << "========== END TRANSACTION HISTORY ============"
  << std::endl;
}

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
