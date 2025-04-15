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

void Voter::submit_vote(const Candidate& candidate){

}

unsigned int Voter::get_age(){return this->age;}

char* Voter::get_voivodship(){return this->voivodship;}

bool Voter::has_voted(){return this->vote;}


Candidate::Candidate(const char* name,const unsigned int age, const char* voivodship,const bool vote,const bool validity,const int support): Voter(name, age, voivodship,vote,validity){

}

void Candidate::vote(){

}

char* Voter::get_name(){

}

unsigned int& Candidate::ref_support(){return this->support;}

void Candidate::display_voters(){

}

Voivodship::Voivodship(const char* name, const unsigned int citizens){
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
    this->citizens=citizens;
}

Voivodship::~Voivodship(){

}

bool Voivodship::register_voter(Voter* voter){

}

void Voivodship::display_registered_voters(){

}

void Voivodship::display_local_support(){

}

unsigned int& Voivodship::number_of_voters(){

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