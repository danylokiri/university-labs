import java.security.*;
import java.util.Base64;

public class Transaction {
    private String inputAddress;
    private String outputAddress;
    private double amount;
    private long txTimestamp;
    private String txHash;
    private String digitalSignature;

    public Transaction(String inputAddress, String outputAddress, double amount, PrivateKey privateKey)
            throws Exception {
        this.inputAddress = inputAddress;
        this.outputAddress = outputAddress;
        this.amount = amount;
        this.txTimestamp = System.currentTimeMillis();
        this.txHash = calculateHash();
        this.digitalSignature = signTransaction(privateKey);
    }

    public String calculateHash() throws Exception {
        String data = inputAddress + outputAddress + amount + txTimestamp;
        MessageDigest digest = MessageDigest.getInstance("SHA-256");
        byte[] hash = digest.digest(data.getBytes());
        return Base64.getEncoder().encodeToString(hash);
    }

    public String signTransaction(PrivateKey privateKey) throws Exception {
        Signature signature = Signature.getInstance("SHA256withECDSA");
        signature.initSign(privateKey);
        signature.update(txHash.getBytes());
        byte[] signedData = signature.sign();
        return Base64.getEncoder().encodeToString(signedData);
    }

    public boolean verifySignature(PublicKey publicKey) throws Exception {
        Signature signature = Signature.getInstance("SHA256withECDSA");
        signature.initVerify(publicKey);
        signature.update(txHash.getBytes());
        byte[] signatureBytes = Base64.getDecoder().decode(digitalSignature);
        return signature.verify(signatureBytes);
    }

    @Override
    public String toString() {
        return "Transaction {\n" +
                "    inputAddress='" + inputAddress + "',\n" +
                "    outputAddress='" + outputAddress + "',\n" +
                "    amount=" + amount + ",\n" +
                "    txTimestamp=" + txTimestamp + ",\n" +
                "    txHash='" + txHash + "',\n" +
                "    digitalSignature='" + digitalSignature + "'\n" +
                "}";
    }
}
