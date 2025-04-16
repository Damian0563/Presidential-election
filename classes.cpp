#include "classes.h"
#include <iostream>
#include <cstring>
using namespace std;

Election::Election(const vector<Voivodship>& voivodship,const unsigned int counter){
    this->voivodships=voivodship;
    this->counter=counter;
    this->headC=nullptr;
}

Election::~Election(){
    Candidates* temp=this->headC;
    Candidates* prev=nullptr;
    while(temp){
        prev=temp;
        prev->candidate->free_supporters();
        delete[] prev->candidate->get_name();
        delete[] prev->candidate->get_voivodship();
        delete prev->candidate;
        delete prev;
        temp=temp->next;
    }
}


void Election::determine_winner(){

}

void Election::support_by_age_group(){

}

bool Election::register_candidate(Candidate* candidate) {
    if(candidate->get_age()>=35){
        Candidates* newNode = new Candidates();
        newNode->candidate = candidate;
        newNode->next = nullptr;
        if(this->headC == nullptr) this->headC = newNode;
        else{
            Candidates* temp = this->headC;
            while(temp->next){
                temp = temp->next;
            }
            temp->next = newNode;
        }
        return true;
    }
    return false;
}

ostream& operator<<(ostream& os, const Candidate& candidate) {
    os << "Candidate Name: " << candidate.name << "\n"
       << "Age: " << candidate.age<< "\n"
       << "Voivodship: " << candidate.voivodship << "\n"
       << "Support: " << candidate.support << "\n";
    return os;
}

void Election::display_registered_candidates(){
    Candidates* temp=this->headC;
    while(temp){
        cout<<temp->candidate;
        temp=temp->next;
    }
}

void Election::distribute_candidates_to_voivodships(){

}

unsigned int& Election::refAttendance() {return this->counter;}

Voter::Voter(const char* name, const unsigned int age,const char* voivovship,bool vote,const bool validity){
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
    this->age=age;
    this->voivodship=new char[strlen(voivodship)+1];
    strcpy(this->voivodship,voivodship);
    this->vote=vote;
    this->validity=validity;
}

Voter::~Voter(){
    delete[] this->name;
    delete[] this->voivodship;
}

void Voter::submit_vote(Candidate& candidate){
    if(this->validity && !this->vote){
        candidate.ref_support()++;
        this->vote=true;
        candidate.add_supporter(this);
    }
}

void Candidate::add_supporter(Voter* voter){
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
    while(temp->next) temp=temp->next;
    temp->next=node;
}

unsigned int Voter::get_age(){return this->age;}

char* Voter::get_voivodship(){return this->voivodship;}

bool Voter::has_voted(){return this->vote;}

Candidate::Candidate(const char* name,const unsigned int age, const char* voivodship,const bool vote,const bool validity,const int support): Voter(name, age, voivodship,vote,validity){
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
    this->age=age;
    this->vote=vote;
    this->voivodship=new char[strlen(voivodship)+1];
    strcpy(this->voivodship,voivodship);
    this->validity=validity;
    this->support=support;
    this->headS=nullptr;
}

void Candidate::submit_vote(){
    if(this->validity && !this->vote){
        ref_support()++;
        this->vote=true;
    }
}

char* Voter::get_name(){return this->name;}

unsigned int& Candidate::ref_support(){return this->support;}

void Candidate::display_voters(){
    Supporters* temp=this->headS;
    while(temp){
        cout<<temp->voter;
        temp=temp->next;
    }
    if(this->validity && this->vote) cout<<this; //Case when president self-voted 
}

Voivodship::Voivodship(const char* name, const unsigned int citizens){
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
    this->citizens=citizens;
    this->headV=nullptr;
}

Voivodship::~Voivodship(){
    this->free_voters();
    this->localVotes.clear();
}

void Voivodship::free_voters(){
    Voters* temp=this->headV;
    Voters* curr=nullptr;
    while(temp){
        curr=temp;
        delete[] curr->voter->get_name();
        delete[] curr->voter->get_voivodship();
        delete curr->voter;
        delete curr;
        temp=temp->next;
    }
}

bool Voivodship::register_voter(Voter* voter){
    if(!this->find(voter->get_name(),voter->get_age())){
        if(strcmp(voter->get_voivodship(),this->name)==0 && voter->get_age()>=18){
            Voters* node=new Voters();
            node->voter=voter;
            node->next=nullptr;
            Voters* temp=this->headV;
            if(temp==nullptr){
                this->headV=node;
            }else {
                while(temp->next) temp=temp->next;
                temp->next=node;
            }
            while(temp->next) temp=temp->next;
            temp->next=node;
            return true;
        }
    }
    return false;
}

void Voivodship::display_registered_voters(){
    Voters* temp=this->headV;
    while(temp){
        cout<<temp->voter;
        temp=temp->next;
    }
}

bool Voivodship::find(const char* name, const unsigned int age) {
    Voters* temp=this->headV;
    while(temp){
        if(strcmp(temp->voter->get_name(), name)==0 && temp->voter->get_age()==age) {
            return true;
        }
        temp=temp->next;
    }
    return false;
}

void Voivodship::display_local_support(){

    for(auto& pair : this->localVotes){
        auto& candidate = pair.first;
        auto& vote = pair.second;
        cout<<candidate;
        cout<<"Number of votes: "<<candidate->has_voted()?vote+1:vote<<'\n';
    }
}

unsigned int& Voivodship::number_of_voters(){
    unsigned int counter=0;
    Voters* temp=this->headV;
    while(temp){
        counter++;
        temp=temp->next;
    }
    return counter;
}

unsigned int Voivodship::number_of_citizens(){
    unsigned int counter=0;
    Voters* temp=this->headV;
    while(temp){
        counter++;
        temp=temp->next;
    }
    return counter;
}

ostream& operator<<(ostream& os,const Voter& voter){
    os << "Candidate Name: " << voter.name << "\n"
       << "Age: " << voter.age<< "\n"
       << "Voivodship: " << voter.voivodship << "\n";
    return os;
}