#include "classes.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

Election::Election(){
    this->headC=nullptr;
}

bool Election::add_voivodship(const char* name, const unsigned int citizens){
    for (auto& v : this->voivodships) {
        if (strcmp(v->get_name(), name) == 0) {
            return false;
        }
    }
    Voivodship* node=new Voivodship(name,citizens);
    this->voivodships.push_back(node);
    return true;
}

Election::~Election(){
    if(this==nullptr) return;
    Candidates* current = this->headC;
    while (current != nullptr) {
        Candidates* temp = current;
        current = current->next;
        if(temp->candidate) delete temp->candidate;
        delete temp;         
    }
    this->headC = nullptr;
}

void Election::determine_winner(){
    if(headC!=nullptr){
        Candidates* temp=this->headC;
        double max=0,curr_support=0,total=0;
        Candidate* winner=this->headC->candidate;
        while(temp){
            curr_support=temp->candidate->ref_support();
            total+=curr_support;
            if(max<curr_support){
                max=curr_support;
                winner=temp->candidate;
            }
            temp=temp->next;
        }
        temp=this->headC;
        double percantage;
        cout<<endl<<"Winner: "<<winner->get_name()<<" gets "<<(max*100)/total<<"%"<<" support in general election."<<endl;
        while(temp){
            if(strcmp(winner->get_name(),temp->candidate->get_name())!=0){
                percantage=((temp->candidate->ref_support())*100)/total;
                cout<<temp->candidate->get_name()<<", support: "<<percantage<<"%"<<endl;
            }
            temp=temp->next;
        }
        cout<<"Election attendance: "<<this->election_attendance()<<"%"<<endl;
    }else cout<<endl<<"No candidates registered"<<endl;
}

void Election::support_by_age_group(){
    if(this->headC){
        Candidates* temp=this->headC;
        double young_adults=0,middle_aged=0,elders=0;
        double total=0;
        if(!temp) cout<<temp->candidate->get_name()<<" has no supporters"<<endl;
        else cout<<endl<<"Support by age group for candidates: "<<endl;
        while(temp){
            total=temp->candidate->ref_support();
            vector<int> age_dist=temp->candidate->age_distribution();
            young_adults=age_dist[0];
            middle_aged=age_dist[1];
            elders=age_dist[2];
            
            cout<<temp->candidate->get_name()<<": "<<endl;
            cout<<"\tYoung adults: "<<(young_adults*100)/total<<"%"<<endl;
            cout<<"\tMiddle aged: "<<(middle_aged*100)/total<<"%"<<endl;
            cout<<"\tElders: "<<(elders*100)/total<<"%"<<endl;
            temp=temp->next;
        }
    }else cout<<"No candidates registered, can not display support by age group"<<endl;
}

void Election::display_registered_candidates(){
    Candidates* temp=this->headC;
    if(!temp) cout<<"No candidates registered"<<endl;
    else cout<<endl<<"Registered candidates: "<<endl;
    while(temp){
        cout<<*(temp->candidate);
        temp=temp->next;
    }
}

double Election::election_attendance(){
    double population=0;
    double voters=0;
    Candidates* temp=this->headC;
    for(auto& voivodship : this->voivodships){
        population+=voivodship->number_of_citizens();
        voters+=voivodship->number_of_submitted_votes();
        while(temp){
            if(strcmp(temp->candidate->get_voivodship(),voivodship->get_name())==0){
                if(temp->candidate->has_voted()){
                    voters++;
                }
                population++;
            }
            temp=temp->next;
        }
        temp=this->headC;
    }
    // cout<<endl<<"Total number of citizens: "<<population<<endl;
    // cout<<"Total number of voters: "<<voters<<endl;
    double percantage=(voters*100)/population;
    return percantage;
}

unsigned int Election::generate_id(){
    static unsigned int last_id = 0; // Keeps track of the last assigned ID
    return ++last_id; // Increment and return a unique ID
}

Voter* Election::register_voter(const char* name, const unsigned int age,const char* voivodship_name){
    if(this->voivodships.empty()) return nullptr; //No voivodships registered
    for(auto& v:this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0 && v->number_of_citizens()>v->number_of_voters()){ //Ensure no duplicate citizens with the sme name and age
            if(age>=18){
                unsigned int id=this->generate_id();;
                Voter* node=new Voter(id,name,age,voivodship_name);
                v->add_voter(node);
                return node;
            }
            return nullptr;
        }
    }
    return nullptr;
}

bool Election::find_candidate(const unsigned int id){
    Candidates* temp=this->headC;
    while(temp){
        if(temp->candidate->get_id()==id) return true;
        temp=temp->next;
    }
    return false;
}

Candidate* Election::register_candidate(const char* name, const unsigned int age, const char* voivodship_name){
    for(auto& v:this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0 && v->number_of_citizens()<=v->number_of_voters()){ //Candidate can not be registered if the number of citizens is less than the number of voters
            return nullptr; 
        }else if(strcmp(v->get_name(),voivodship_name)==0 && age>=35){
            v->decrease_voter_count(); //Decrement the number of citizens
        }
    }
    if(age>=35){
        unsigned int id=this->generate_id();
        Candidate* node=new Candidate(id,name,age,voivodship_name);
        Candidates* temp=this->headC;
        if(!temp) {
            this->headC = new Candidates();
            this->headC->candidate = node;
            this->headC->next = nullptr;
            return node;
        }
        while(temp->next){
            temp=temp->next;
        }
        temp->next = new Candidates();
        temp->next->candidate = node;
        temp->next->next = nullptr;
        return node;
    }
    return nullptr;
}

void Election::display_all_voters(){
    for(auto& v : this->voivodships){
        v->display_voters();
    }
}

bool Election::display_registered_voters(const char* voivodship_name){
    for(auto& v : this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0){
            v->display_voters();
            cout<<"Number of voters: "<<v->number_of_voters()<<", Number of citizens: "<<v->number_of_citizens()<<endl;
            return true;
        }
    }
    return false;
}

bool Election::display_local(const char* voivodship_name){
    for(auto& v : this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0){
            cout<<endl<<v->get_name()<<": "<<endl;
            Candidates* temp=this->headC;
            while(temp){
                unsigned int increment1=0,increment2=this->get_votes_of_candidates(voivodship_name);
                if(strcmp(temp->candidate->get_voivodship(),voivodship_name)==0 && temp->candidate->has_voted()){
                    increment1++;
                }
                //cout<<temp->candidate->supporters_in_voivodship(voivodship_name)<<"+"<<increment1<<", "<<v->number_of_submitted_votes()<<"+"<<increment2<<endl;
                cout<<temp->candidate->get_name()<<": "<<(double(temp->candidate->supporters_in_voivodship(voivodship_name)+increment1)*100)/double(v->number_of_submitted_votes()+increment2)<<"%"<<endl;
                temp=temp->next;
            }
            return true;
        }
    }
    return false;
}

unsigned int Election::get_votes_of_candidates(const char* voivodship_name){
    for(auto& v : this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0){
            Candidates* temp=this->headC;
            int counter=0;
            while(temp){
                if(strcmp(temp->candidate->get_voivodship(),v->get_name())==0 && temp->candidate->has_voted()){
                    counter++;
                }
                temp=temp->next;
            }
            return counter;
        }
    }
    return 0;
}

bool Election::local_support(const Candidate* candidate,const char* voivodship_name){
    for(auto& v : this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0){
            cout<<endl<<v->get_name()<<": "<<endl;
            Candidates* temp=this->headC;
            while(temp){
                if(strcmp(temp->candidate->get_name(),candidate->get_name())==0){
                    int  increment1=0;
                    if(strcmp(temp->candidate->get_voivodship(),voivodship_name)==0){
                        increment1++;
                    }
                    int increment2=this->get_votes_of_candidates(voivodship_name);
                    //cout<<temp->candidate->supporters_in_voivodship(voivodship_name)<<"+"<<increment1<<", "<<v->number_of_submitted_votes()<<"+"<<increment2<<endl;
                    cout<<temp->candidate->get_name()<<": "<<(double(temp->candidate->supporters_in_voivodship(voivodship_name)+increment1)*100)/double(v->number_of_submitted_votes()+increment2)<<"%"<<endl;
                    return true;
                }
                temp=temp->next;
            }
        }
    }
    return false;
}

void Election::display_voters(const Candidate* candidate){
    candidate->display_supporters();
}

void Election::display_voivodships(){
    for(auto& v : this->voivodships){
        cout<<v->get_name()<<", ";
    }
    cout<<endl;
}

bool Election::die_voter(unsigned int voter_id){
    Candidates* temp=this->headC;
    while(temp){
        temp->candidate->delete_supporter(voter_id);
        temp=temp->next;
    }
    for(auto& v : this->voivodships){
        v->delete_voter(voter_id);
        return true;
    }
    return false;
}

bool Election::die_candidate(unsigned int candidate_id){
    if(!this->find_candidate(candidate_id)) return false;
    Candidates* temp=this->headC;
    Candidates* prev=nullptr;
    while(temp){
        if(temp->candidate->get_id()==candidate_id){
            if(prev) prev->next=temp->next;
            else this->headC=temp->next;
            if(temp->candidate) delete temp->candidate;
            if(temp) delete temp;
            return true;
        }
        prev=temp;
        temp=temp->next;
    }
    return false;
}

unsigned int Election::get_number_of_citizens(const char* voivodship_name){
    for(auto& v : this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0){
            return v->number_of_citizens();
        }
    }
    return 0;
}


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
    if(this==nullptr) return;
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
    if(this==nullptr) return;
    Supporters* current=this->headS;
    while (current!=nullptr) {
        Supporters* temp=current;
        current=current->next;
        if(temp->voter) delete temp->voter;
        delete temp;
    }
    this->headS=nullptr;
}

void Candidate::submit_vote(){
    if(this==nullptr) return;
    ref_support()++;
    this->has_voted()=true;
}

unsigned int& Candidate::ref_support(){
    if(this==nullptr){
        static unsigned int x = 0;
        return x;
    } 
    return this->support;
}

vector<int> Candidate::age_distribution(){
    if (this==nullptr) return {0,0,0};
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
    if(this==nullptr) return;
    Supporters* temp=this->headS;
    cout<<endl<<this->get_name()<<" supporters: "<<endl;
    while(temp){
        cout<<*(temp->voter);
        temp=temp->next;
    }
}

unsigned int Candidate::supporters_in_voivodship(const char* voivodship){
    if (this==nullptr) return 0;
    unsigned int counter=0;
    Supporters* temp=this->headS;
    while(temp){
        if(strcmp(temp->voter->get_voivodship(),voivodship)==0) counter++;
        temp=temp->next;
    }
    return counter;
}

void Candidate::delete_supporter(unsigned int voter_id){
    if(this==nullptr) return;
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
    if (this==nullptr){
        static unsigned int x = 0;
        return x;
    }
    return this->age;
}

char* Voter::get_voivodship()const {
    if(this==nullptr){
        static char* x = const_cast<char*>("nullptr");
        return x;
    }
    return this->voivodship;
}

bool& Voter::has_voted(){
    if(this==nullptr){
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
    if (this==nullptr){
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

void Voter::submit_vote(Candidate& candidate){
    if(this==nullptr) return;
    if(!this->has_voted()){
        candidate.ref_support()++;
        this->has_voted()=true;
        candidate.add_supporter(this);
    }
}

bool Voter::get_vote()const {
    if(this==nullptr) return false;
    return this->vote;
}

unsigned int Voter::get_id()const {
    if(this==nullptr) return 0;
    return this->id;
}



Voivodship::Voivodship(const char* name, const unsigned int citizens){
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
    this->citizens=citizens;
    this->headV=nullptr;
}

Voivodship::~Voivodship(){
    this->free_voters();
}

char* Voivodship::get_name(){return this->name;}

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

unsigned int Voivodship::number_of_voters(){
    unsigned int counter=0;
    Voters* temp=this->headV;
    while(temp){
        counter++;
        temp=temp->next;
    }
    return counter;
}

unsigned int Voivodship::number_of_citizens(){
    return this->citizens;
}

void Voivodship::free_voters(){
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
    Voters* temp=this->headV;
    cout<<endl<<this->name<<" voters: "<<endl;
    while(temp){
        cout<<*(temp->voter);
        temp=temp->next;
    }
}

void Voivodship::decrease_voter_count(){
    this->citizens--;
}

unsigned int Voivodship::number_of_submitted_votes(){
    unsigned int counter=0;
    Voters* temp=this->headV;
    while(temp){
        if(temp->voter->has_voted()) counter++;
        temp=temp->next;
    }
    return counter;
}

bool Voivodship::add_voter(Voter* voter){
    Voters* temp = this->headV;
    if(this->find(voter->get_name(), voter->get_age())) {
        return false;
    }
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
    this->citizens++;
}

void Voivodship::delete_voter(unsigned int voter_id){
    Voters* temp=this->headV;
    Voters* prev=nullptr;
    while(temp){
        if(temp->voter->get_id()==voter_id){
            if(prev) prev->next=temp->next;
            else this->headV=temp->next;
            if(temp->voter) delete temp->voter;
            if(temp) delete temp;
            this->decrease_voter_count();
            return;
        }
        prev=temp;
        temp=temp->next;
    }
}