#include <iostream>
#include <memory>
#include <vector>


class Client;
class Server;


class Client {

private:
	std::shared_ptr< Server > server_;
	std::string name_;

public:
	Client(std::shared_ptr<Server> server, std::string name) : server_(server), name_(name) {
		puts("Create a Client");
	}

	~Client() {
		puts("Destory a Client");
	}

	std::string GetName() {
		return this->name_;
	}

};

class Server {

private:
	std::vector< std::weak_ptr<Client>> clients_;

public:
	Server() {
		puts("Create a Server");
	}


	~Server() {
		puts("Destory a Server");
	}

	void AddClients(const std::shared_ptr<Client> client_ptr) {
		clients_.push_back(client_ptr);
	}


	void ShowClintInfo() {
		for (auto weak_ptr : clients_) {
			std::shared_ptr<Client> client_ptr = weak_ptr.lock();
			if (client_ptr) {
				printf("Clinet Name: %s \n" , client_ptr->GetName());
				
			}
		}
	}
};






int main(){


	std::shared_ptr<Server> server_ptr = std::make_shared<Server>(); 
	printf("Before Clients were created  : %d \n", server_ptr.use_count()); 
	std::shared_ptr<Client> client_a_ptr = std::make_shared<Client>( server_ptr , "charles");
	std::shared_ptr<Client> client_b_ptr = std::make_shared<Client>( server_ptr , "hank");
	printf("After Clients were created   : %d \n", server_ptr.use_count());





	printf("Before Client_a  was added to Server : %d \n", client_a_ptr.use_count());
	server_ptr->AddClients(client_a_ptr);
	printf("After Client_a  was added to Server : %d \n", client_a_ptr.use_count());


	printf("Before Client_b  was added to Server : %d \n", client_b_ptr.use_count());
	server_ptr->AddClients(client_b_ptr);
	printf("After Client_b  was added to Server : %d \n", client_b_ptr.use_count());

	server_ptr -> ShowClintInfo();
}