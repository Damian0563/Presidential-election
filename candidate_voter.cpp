#include <iostream>
#include <cstring>
#include "classes.h"
using namespace std;

ostream& operator<<(ostream& os, const Candidate& candidate) {
    if(&candidate==nullptr) return os;
    os <<"\n"
    << "Candidate Name: " << candidate.get_name() << "\n"
    << "ID: " << candidate.get_id() << "\n"
    << "Age: " << candidate.get_age()<< "\n"
    << "Voivodship: " << candidate.get_voivodship() << "\n"
    << "Support: " << candidate.support << "\n";
    return os;
    
}

void Candidate::add_supporter(Voter* voter){
    if(!this) return;
    Supporters* node=new Supporters();
    node->voter=voter;
    node->next=nullptr;
    Supporters* temp=this->headS;
    if(temp==nullptr){
        this->headS=node;
    }else {
        while(temp->next) temp=temp->next;
        temp->next=node;
    }
}

Candidate::Candidate(const unsigned int id,const char* name, const unsigned int age, const char* voivodship, const bool vote, const int support)
    : Voter(id,name, age, voivodship, vote), support(support), headS(nullptr) {}

void Candidate::free_supporters(){
    if(!this) return;
    this->headS=nullptr;
}

void Candidate::submit_vote(){
    if(!this) return;
    ref_support()++;
    this->has_voted()=true;
}

unsigned int& Candidate::ref_support(){
    if(!this){
        static unsigned int x = 0;
        return x;
    } 
    return this->support;
}

vector<int> Candidate::age_distribution(){
    if (!this) return {0,0,0};
    int young_adults=0,middle_aged=0,elders=0;
    Supporters* temp=this->headS;
    while(temp){
        if(temp->voter->get_age()>=18 && temp->voter->get_age()<40) young_adults++;
        else if(temp->voter->get_age()>=40 && temp->voter->get_age()<65) middle_aged++;
        else elders++;
        temp=temp->next;
    }
    if(this->get_vote()){
        if(this->get_age()>=18 && this->get_age()<40) young_adults++;
        else if(this->get_age()>=40 && this->get_age()<65) middle_aged++;
        else elders++;
    }
    //cout<<young_adults<<"+"<<middle_aged<<"+"<<elders<<endl;
    return {young_adults,middle_aged,elders};
}

Candidate::~Candidate(){
    if(this!=nullptr){
        this->free_supporters();
    }
}

void Candidate::display_supporters() const {
    if(!this) return;
    Supporters* temp=this->headS;
    cout<<endl<<this->get_name()<<" supporters: "<<endl;
    while(temp){
        cout<<*(temp->voter);
        temp=temp->next;
    }
}

unsigned int Candidate::supporters_in_voivodship(const char* voivodship){
    if (!this) return 0;
    unsigned int counter=0;
    Supporters* temp=this->headS;
    while(temp){
        if(strcmp(temp->voter->get_voivodship(),voivodship)==0) counter++;
        temp=temp->next;
    }
    return counter;
}

void Candidate::delete_supporter(unsigned int voter_id){
    if(!this) return;
    Supporters* temp=this->headS;
    Supporters* prev=nullptr;
    while(temp){
        if(temp->voter->get_id()==voter_id){
            if(prev) prev->next=temp->next;
            else this->headS=temp->next;
            if(temp->voter) delete temp->voter;
            if(temp) delete temp;
            return;
        }
        prev=temp;
        temp=temp->next;
    }
}



unsigned int Voter::get_age()const {
    if (!this){
        static unsigned int x = 0;
        return x;
    }
    return this->age;
}

char* Voter::get_voivodship()const {
    if(!this){
        static char* x = const_cast<char*>("nullptr");
        return x;
    }
    return this->voivodship;
}

bool& Voter::has_voted(){
    if(!this){
        static bool x = false;
        return x;
    }
    return this->vote;
}

ostream& operator<<(ostream& os,const Voter& voter){
    if(&voter==nullptr) return os;
    os << "Voter Name: " << voter.name << "\n"
        << "ID: " << voter.id << "\n"
       << "Age: " << voter.age<< "\n"
       << "Voivodship: " << voter.voivodship << "\n";
    return os;
}

char* Voter::get_name()const {
    if (!this){
        static char* x = const_cast<char*>("nullptr"); 
        return x;
    }
    return this->name;
}

Voter::Voter(const unsigned int id,const char* name, const unsigned int age,const char* voivodship,bool vote){
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
    this->id=id;
    this->age=age;
    this->voivodship=new char[strlen(voivodship)+1];
    strcpy(this->voivodship,voivodship);
    this->vote=vote;
}

Voter::~Voter(){
    if(this != nullptr) {
        delete[] this->name;
        delete[] this->voivodship;
    }
}

void Voter::submit_vote(Election* e,Candidate& candidate){
    if(!e->find_candidate(candidate.get_id())) return;
    if(this->get_id()==0) return;
    if(!this->has_voted()){
        candidate.ref_support()++;
        this->has_voted()=true;
        candidate.add_supporter(this);
    }
}

bool Voter::get_vote()const {
    if(!this) return false;
    return this->vote;
}

unsigned int Voter::get_id()const {
    if(!this) return 0;
    return this->id;
}

unsigned int& Voter::refId(){
    if(!this) return id;
    return this->id;
}