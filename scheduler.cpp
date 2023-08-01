#include <string>
#include <iostream>
#include <fstream>
#include <deque>

const int TIME_ALLOWANCE = 6;

using namespace std;
/*
 * customer class that stores the basic definition
 */
class Customer
{
public:
	int arrival_time;
	int customer_id;
	int priority;
	int slots_remaining;
	int waiting_time;
	float corresponding_ratio; //(waiting_time+remaining_time)/remaining_time, the higher the more emergent
	int tasteHours;			   // for each customer they has privileged 6 taste hours:once they arrive, they can enjoy a hour almost immediately(if at that time only one customer arrives)

	Customer(string id, int priority, int arrival_time, int slots_required)
	{
		customer_id = stoi(id.substr(1)); // strip the 'c' part and store the id as int
		this->priority = priority;
		this->arrival_time = arrival_time;
		this->slots_remaining = slots_required;
		waiting_time = 0;
		tasteHours = 6;
	}
	Customer(){};
};

/*
 * read input from a ifstream, store the customer information in a deque named as customers
 */
void read_input(deque<Customer> &customers, ifstream &in_file)
{
	string name;
	int arrival_time, priority, slots_required;

	while (in_file >> name >> priority >> arrival_time >> slots_required)
	{
		Customer customer(name, priority, arrival_time, slots_required);
		customers.push_back(customer);
	}
}

/*
 * for all customers in the queue:
 * a. add waiting time by one
 * b. calculate responding ratio for each customers in the queue
 */
void refresh_waiting_and_corres(deque<Customer> &queue)
{
	for (Customer &customer : queue)
	{
		customer.waiting_time++;
		customer.corresponding_ratio = (static_cast<float>(customer.waiting_time) + customer.slots_remaining) / customer.slots_remaining;
	}
}

/*
 * sort the queue by their corresponding ratio, from highest to lowest
 * insertion sort
 */
void sort_queue_by_corresponding_ratio(deque<Customer> &queue)
{
	if (queue.size() <= 1)
	{
		return;
	}
	Customer key;
	int pos;
	for (int i = 1; i < queue.size(); i++)
	{
		key = queue[i];
		pos = i;
		for (int j = i - 1; j >= 0; j--)
		{
			if (queue[j].corresponding_ratio < key.corresponding_ratio)
			{
				queue[j + 1] = queue[j];
				pos = j;
			}
		}
		queue[pos] = key;
	}
}
/*
 * refresh queue:refresh waiting time and corresponding ratio and sort the queue accordingly
 */
void refresh_queue(deque<Customer> &queue)
{
	refresh_waiting_and_corres(queue);
	sort_queue_by_corresponding_ratio(queue);
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cerr << "Provide input and output file names." << std::endl;
		return -1;
	}
	std::ifstream in_file(argv[1]);
	ofstream out_file(argv[2]);
	if ((!in_file) || (!out_file))
	{
		cerr << "Cannot open one of the files." << std::endl;
		return -1;
	}
	// exceptions about given argument

	deque<Customer> customers, taste, normal_queue, vip_queue, playing;
	read_input(customers, in_file);
	// get customers' information

	int time = 0;
	while (!taste.empty() || !normal_queue.empty() || !vip_queue.empty() || !customers.empty() || !playing.empty())
	{
		while (!customers.empty() && customers[0].arrival_time == time)
		{
			taste.push_back(customers[0]);
			customers.pop_front();
		}
		// when customers arrive, they line in the taste queue first

		if (!playing.empty() && !taste.empty() && playing[0].tasteHours <= 0)
		{
			if (playing[0].priority == 0)
			{
				vip_queue.push_back(playing[0]);
			}
			else
			{
				normal_queue.push_back(playing[0]);
			}
			playing.pop_front();
			// send the one priorly using the machine back to normal queue and vip queue

			playing.push_back(taste[0]);
			taste.pop_front();
			// the one at the head of taste queue begins to use the machine
		}
		// case that we need interrupt: someone with their taste hour unused is waiting and the current one who is using the machine is not in their taste hours

		if (playing.empty())
		{
			if (!taste.empty())
			{
				playing.push_back(taste[0]);
				taste.pop_front();
			}
			else if (!vip_queue.empty())
			{
				playing.push_back(vip_queue[0]);
				vip_queue.pop_front();
			}
			else if (!normal_queue.empty())
			{
				playing.push_back(normal_queue[0]);
				normal_queue.pop_front();
			}
		}
		// when the playing machine is empty, pop the customer out in such order:
		// taste_queue>>vip_queue>>normal_queue

		int id = playing.empty() ? -1 : playing[0].customer_id;
		out_file << time << " " << id << '\n';
		// write the current status to the file

		time++;
		// time elapses

		if (!playing.empty())
		{
			playing[0].slots_remaining--;
			if (playing[0].tasteHours > 0)
			{
				playing[0].tasteHours--;
			}

			if (playing[0].slots_remaining == 0)
			{
				playing.pop_back();
			}
			// if the one occupying the machine is done playing
			else if (playing[0].tasteHours == 0)
			{
				if (playing[0].priority == 0)
				{
					vip_queue.push_back(playing[0]);
				}
				else
				{
					normal_queue.push_back(playing[0]);
				}
				playing.pop_back();
			}
			// if the one occupying the machine has used their taste hours, send them to the normal queue or vip queue
		}
		refresh_queue(vip_queue);
		refresh_queue(normal_queue);
		// what happpens in each time slot
	}
	out_file << time << " " << -1 << endl;
	// end of writing
	return 0;
}