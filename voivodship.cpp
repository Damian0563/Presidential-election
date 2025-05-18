#include <iostream>
#include <cstring>
#include "classes.h"
using namespace std;

Voivodship::Voivodship(const char* name, const unsigned int citizens){
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
    this->citizens=citizens;
    this->headV=nullptr;
}

Voivodship::~Voivodship(){
    if(!this) return;
    this->free_voters();
}

char* Voivodship::get_name(){
     if (!this){
        static char* x = const_cast<char*>("nullptr"); 
        return x;
    }
    return this->name;
}

bool Voivodship::find(const unsigned int id) {
    if(!this) return false;
    Voters* temp=this->headV;
    while(temp){
        if(temp->voter->get_id()==id) return true;
        temp=temp->next;
    }
    return false;
}

unsigned int Voivodship::number_of_voters(){
    if(!this) return 0;
    unsigned int counter=0;
    Voters* temp=this->headV;
    while(temp){
        if(temp->voter->get_id()!=0 && !temp->voter->get_vote()) counter++;
        temp=temp->next;
    }
    return counter;
}

unsigned int Voivodship::number_of_citizens(){
    if(!this) return 0;
    return this->citizens;
}

void Voivodship::free_voters(){
    if(!this) return;
    Voters* current=this->headV;
    while (current!=nullptr) {
        Voters* temp=current;
        current=current->next;
        if(temp->voter) delete temp->voter;
        delete temp;
    }
    this->headV=nullptr;
}

void Voivodship::display_voters(){
    if(!this) return;
    Voters* temp=this->headV;
    if(temp) cout<<endl<<this->name<<" voters: "<<endl;
    else cout<<"No voters in "<<this->name<<endl;
    while(temp){
        cout<<*(temp->voter);
        temp=temp->next;
    }
}

void Voivodship::decrease_voter_count(){
    if(!this) return;
    this->citizens--;
}

unsigned int Voivodship::number_of_submitted_votes(){
    if(!this) return 0;
    unsigned int counter=0;
    Voters* temp=this->headV;
    while(temp){
        if(temp->voter->has_voted()) counter++;
        temp=temp->next;
    }
    return counter;
}

bool Voivodship::add_voter(Voter* voter){
    if(!this || !voter) return false;
    Voters* temp = this->headV;
    if(this->number_of_citizens()<=this->number_of_voters()){
        return false;
    }
    Voters* node = new Voters();
    node->voter = voter;
    node->next = this->headV;
    this->headV = node;
    return true;
}

void Voivodship::increase_voter_count(){
    if(!this) return;
    this->citizens++;
}

Voter* Voivodship::get_voter(unsigned int id){
    if(!this) return nullptr;
    Voters* temp=this->headV;
    while(temp){
        if(temp->voter->get_id()==id) return temp->voter;
        temp=temp->next;
    }
    return nullptr;
}

bool Voivodship::delete_voter(unsigned int voter_id){
    if(!this || !voter_id) return false;
    Voters* temp=this->headV;
    Voters* prev=nullptr;
    while(temp){
        if(temp->voter->get_id()==voter_id){
            if(prev) prev->next=temp->next;
            else this->headV=temp->next;
            if(temp->voter) delete temp->voter;
            if(temp) delete temp;
            this->decrease_voter_count();
            return true;
        }
        prev=temp;
        temp=temp->next;
    }
    return false;
}