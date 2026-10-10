import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

class Transaction {
    String sender;
    String receiver;
    double amount;

    public Transaction(String sender, String receiver, double amount) {
        this.sender = sender;
        this.receiver = receiver;
        this.amount = amount;
    }
}

class Block {
    String previousHash;
    String merkleRoot;
    long timeStamp;
    int nonce;
    String hash;
    List<Transaction> transactions;

    public Block(String previousHash, List<Transaction> transactions) {
        this.previousHash = previousHash;
        this.transactions = transactions;
        this.timeStamp = new Date().getTime();
        this.merkleRoot = calculateMerkleRoot();
        this.hash = calculateHash();
    }

    public String calculateHash() {
        String dataToHash = previousHash + timeStamp + nonce + merkleRoot;
        return applySha256(dataToHash);
    }

    public void mineBlock(int difficulty) {
        String target = new String(new char[difficulty]).replace('\0', '0');
        while (!hash.substring(0, difficulty).equals(target)) {
            nonce++;
            hash = calculateHash();
        }
        System.out.println("Block Mined: " + hash);
    }

    private String calculateMerkleRoot() {
        List<String> hashes = new ArrayList<>();
        for (Transaction transaction : transactions) {
            hashes.add(applySha256(transaction.sender + transaction.receiver + transaction.amount));
        }
        while (hashes.size() > 1) {
            List<String> newHashes = new ArrayList<>();
            for (int i = 0; i < hashes.size(); i += 2) {
                if (i + 1 < hashes.size()) {
                    newHashes.add(applySha256(hashes.get(i) + hashes.get(i + 1)));
                } else {
                    newHashes.add(hashes.get(i));
                }
            }
            hashes = newHashes;
        }
        return hashes.size() == 1 ? hashes.get(0) : "";
    }

    public static String applySha256(String input) {
        try {
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            byte[] hash = digest.digest(input.getBytes("UTF-8"));
            StringBuilder hexString = new StringBuilder();
            for (byte b : hash) {
                String hex = Integer.toHexString(0xff & b);
                if (hex.length() == 1)
                    hexString.append('0');
                hexString.append(hex);
            }
            return hexString.toString();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}

class Node {
    Blockchain blockchain;

    public Node(Blockchain blockchain) {
        this.blockchain = blockchain;
    }

    public void mineBlock(List<Transaction> transactions) {
        String previousHash = blockchain.getLastBlock().hash;
        Block newBlock = new Block(previousHash, transactions);
        newBlock.mineBlock(Blockchain.difficulty);
        blockchain.addBlock(newBlock);
    }
}

class Blockchain {
    public static List<Block> blockchain = new CopyOnWriteArrayList<>();
    public static int difficulty = 5;

    public Blockchain() {
        List<Transaction> initialTransactions = new ArrayList<>();
        initialTransactions.add(new Transaction("System", "User1", 50));
        Block genesisBlock = new Block("0", initialTransactions);
        genesisBlock.mineBlock(difficulty);
        blockchain.add(genesisBlock);
    }

    public Block getLastBlock() {
        return blockchain.get(blockchain.size() - 1);
    }

    public void addBlock(Block block) {
        blockchain.add(block);
    }

    public static boolean isChainValid() {
        for (int i = 1; i < blockchain.size(); i++) {
            Block currentBlock = blockchain.get(i);
            Block previousBlock = blockchain.get(i - 1);

            if (!currentBlock.hash.equals(currentBlock.calculateHash())) {
                return false;
            }

            if (!currentBlock.previousHash.equals(previousBlock.hash)) {
                return false;
            }
        }
        return true;
    }
}

public class BlockchainNetwork {
    public static void main(String[] args) {
        Blockchain blockchain = new Blockchain();

        Node node1 = new Node(blockchain);
        Node node2 = new Node(blockchain);
        Node node3 = new Node(blockchain);

        List<Transaction> transactions1 = new ArrayList<>();
        transactions1.add(new Transaction("User1", "User2", 20));
        node1.mineBlock(transactions1);

        List<Transaction> transactions2 = new ArrayList<>();
        transactions2.add(new Transaction("User2", "User3", 30));
        node2.mineBlock(transactions2);

        List<Transaction> transactions3 = new ArrayList<>();
        transactions3.add(new Transaction("User3", "User1", 10));
        node3.mineBlock(transactions3);

        // Проверка валидности цепочки
        System.out.println("Blockchain is valid: " + Blockchain.isChainValid());
    }
}
