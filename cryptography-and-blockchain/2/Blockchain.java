import java.util.ArrayList;
import java.util.List;

public class Blockchain {
    private List<Block> chain;

    public Blockchain() {
        this.chain = new ArrayList<>();
        chain.add(createGenesisBlock());
    }

    private Block createGenesisBlock() {
        Block genesisBlock = new Block(1, "0", 1);
        genesisBlock.setBlockHash(genesisBlock.calculateBlockHash());
        return genesisBlock;
    }

    public Block getLastBlock() {
        return chain.get(chain.size() - 1);
    }

    public void addBlock(Block newBlock) {
        newBlock.setPrevHash(getLastBlock().getBlockHash());
        newBlock.setBlockHash(newBlock.calculateBlockHash());
        chain.add(newBlock);
    }

    public void printBlockchain() {
        for (Block block : chain) {
            System.out.println(block);
        }
    }

    public List<Block> getChain() {
        return chain;
    }
}
