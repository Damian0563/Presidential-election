#include "classes.h"
#include <cstring>
using namespace std;

Election::Election(const vector<Voivodship>& voivodship,const unsigned int counter=0){
    this->voivodships=voivodship;
}

Election::~Election(){

}

void Election::determine_winner(){

}

void Election::support_by_age_group(){

}

bool Election::register_candidate(Candidate* candidate){

}

void Election::display_registered_candidates(){

}

void Election::distribute_candidates_to_voivodships(){

}

unsigned int& Election::refAttendance() {return this->counter;}

Voter::Voter(const char* name, const unsigned int age,const char* voivovship,bool vote=false,const bool validity=false){
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

void Voter::display(){

}

Candidate::Candidate(const char* name,const unsigned int age, const char* voivodship,const bool vote=false,const bool validity=false,const int support=0): Voter(name, age, voivodship,vote,validity){

}

void Candidate::vote(){

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

int Voivodship::number_of_voters(){

}

unsigned int& Voivodship::number_of_citizens(){

}