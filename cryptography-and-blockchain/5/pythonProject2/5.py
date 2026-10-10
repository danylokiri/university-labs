import threading
import time
import matplotlib.pyplot as plt

class Node:
    def __init__(self, node_id, total_nodes):
        self.node_id = node_id
        self.blockchain = []
        self.valid_votes = 0
        self.total_nodes = total_nodes

    def validate_block(self, block):
        return True

    def receive_block(self, block, nodes):
        if self.validate_block(block):
            print(f"Node {self.node_id}: Block is valid. Sending 'Valid' to other nodes...")
            self.broadcast_valid_message(nodes)

    def broadcast_valid_message(self, nodes):
        time.sleep(0.1)
        for node in nodes:
            if node.node_id != self.node_id:
                node.receive_valid_message()

    def receive_valid_message(self):
        self.valid_votes += 1

    def add_block_to_blockchain(self, block):
        self.blockchain.append(block)
        print(f"Node {self.node_id}: Block added to blockchain.")

def simulate_bft_protocol(node_count):
    nodes = [Node(i, node_count) for i in range(node_count)]
    leader = nodes[0]
    block = "Block 1"

    print("Leader: Creating block and sending to all nodes...")
    threads = []
    for node in nodes[1:]:
        thread = threading.Thread(target=node.receive_block, args=(block, nodes))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

    for node in nodes:
        if node.valid_votes >= (2 * node_count) // 3:
            node.add_block_to_blockchain(block)

if __name__ == "__main__":
    participants = [10, 100, 1000]
    total_results = []

    for n in participants:
        print(f"\nSimulating BFT protocol with {n} nodes...")
        start_time = time.time()
        simulate_bft_protocol(n)
        end_time = time.time()
        execution_time = end_time - start_time
        total_results.append((n, execution_time))
        print(f"Execution time for {n} nodes: {execution_time:.2f} seconds")

    print("\nSummary of execution times:")
    for result in total_results:
        print(f"{result[0]} nodes: {result[1]:.2f} seconds")

    nodes_counts = [result[0] for result in total_results]
    execution_times = [result[1] for result in total_results]

    plt.figure(figsize=(10, 6))
    plt.plot(nodes_counts, execution_times, marker='o', linestyle='-', color='b')
    plt.xlabel('Number of Nodes')
    plt.ylabel('Execution Time (seconds)')
    plt.title('Execution Time vs Number of Nodes for BFT Protocol')
    plt.grid(True)
    plt.show()
