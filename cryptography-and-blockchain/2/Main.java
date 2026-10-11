import java.security.KeyPair;
import java.security.KeyPairGenerator;

public class Main {
    public static void main(String[] args) throws Exception {
        KeyPairGenerator keyGen = KeyPairGenerator.getInstance("EC");
        keyGen.initialize(256);
        KeyPair pair = keyGen.generateKeyPair();

        Blockchain blockchain = new Blockchain();
        Block block = new Block(1, blockchain.getLastBlock().getBlockHash(), 1);

        Transaction tx1 = new Transaction("Alice", "Bob", 10.0, pair.getPrivate());
        block.addTransaction(tx1);

        blockchain.addBlock(block);
        blockchain.printBlockchain();
    }
}
