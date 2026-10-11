import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.Base64;
import java.util.List;

public class Block {
    private int version;
    private String prevHash;
    private long timestamp;
    private int difficultyTarget;
    private int nonce;
    private String merkleRoot;
    private List<Transaction> transactions;
    private String blockHash;

    public Block(int version, String prevHash, int difficultyTarget) {
        this.version = version;
        this.prevHash = prevHash;
        this.timestamp = System.currentTimeMillis();
        this.difficultyTarget = difficultyTarget;
        this.transactions = new ArrayList<>();
        this.nonce = 0;
    }

    public void addTransaction(Transaction transaction) {
        this.transactions.add(transaction);
        this.merkleRoot = calculateMerkleRoot();
        this.blockHash = calculateBlockHash();
    }

    public String calculateMerkleRoot() {
        StringBuilder concatenatedHashes = new StringBuilder();
        for (Transaction transaction : transactions) {
            try {
                concatenatedHashes.append(transaction.calculateHash());
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return hashSHA256(concatenatedHashes.toString());
    }

    public String calculateBlockHash() {
        String data = version + prevHash + timestamp + difficultyTarget + nonce + merkleRoot;
        return hashSHA256(data);
    }

    private String hashSHA256(String data) {
        try {
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            byte[] hash = digest.digest(data.getBytes());
            return Base64.getEncoder().encodeToString(hash);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public String getPrevHash() {
        return prevHash;
    }

    public void setPrevHash(String prevHash) {
        this.prevHash = prevHash;
    }

    public String getBlockHash() {
        return blockHash;
    }

    public void setBlockHash(String blockHash) {
        this.blockHash = blockHash;
    }

    @Override
    public String toString() {
        return "Block {\n" +
                "  version=" + version + ",\n" +
                "  prevHash='" + prevHash + "',\n" +
                "  timestamp=" + timestamp + ",\n" +
                "  difficultyTarget=" + difficultyTarget + ",\n" +
                "  nonce=" + nonce + ",\n" +
                "  merkleRoot='" + merkleRoot + "',\n" +
                "  transactions=" + transactions + ",\n" +
                "  blockHash='" + blockHash + "'\n" +
                "}";
    }
}
