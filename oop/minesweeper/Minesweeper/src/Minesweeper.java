import java.awt.*;
import java.awt.event.*;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Random;
import java.util.Scanner;
import java.util.Set;
import javax.swing.*;

public class Minesweeper {

    String resultsFileName = "minesweeper_results.txt";
    private static Minesweeper minesweeperInstance;

    private class MineTile extends JButton {
        int r;
        int c;

        public MineTile(int r, int c) {
            this.r = r;
            this.c = c;
            setPreferredSize(new Dimension(30, 30));
            setFocusPainted(false);
        }
    }

    int tileSize = 30;
    int numRows = 5000;
    int numCols = numRows;
    int boardWidth;
    int boardHeight;
    boolean gameWon = false;
    boolean timerStarted = false;

    JFrame frame = new JFrame("Minesweeper");
    JPanel textPanel = new JPanel();
    JPanel boardPanel = new JPanel();
    JMenuBar menuBar = new JMenuBar();
    JMenu gameS = new JMenu("Game");
    JMenu helP = new JMenu("Help");
    JMenu boardEdit = new JMenu("Change Size");
    JMenuItem restart = new JMenuItem("Restart");
    JMenuItem scoreBoard = new JMenuItem("Scoreboard");
    JMenuItem size8x8 = new JMenuItem("Easy level | 8x8");
    JMenuItem size16x16 = new JMenuItem("Medium level | 16x16");
    JMenuItem size32x32 = new JMenuItem("Expert level | 16x30");
    JMenuItem sizeCustom = new JMenuItem("Custom size");
    JMenuItem howPlay = new JMenuItem("How to play");
    JMenuItem about = new JMenuItem("About");

    int mineCount = 12;

    MineTile[][] board = new MineTile[numRows][numCols];
    ArrayList<MineTile> mineList;
    Random random = new Random();

    int tilesClicked = 0;
    boolean gameOver = false;
    boolean gameStarted = false;
    long startTime;
    Timer timer;
    private MineTile lastClickedTile;

    Minesweeper() {
        minesweeperInstance = this;
        try {
            GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
            ge.registerFont(Font.createFont(Font.TRUETYPE_FONT, new File("res/fonts/pixelfont.ttf")));
        } catch (IOException | FontFormatException e) {
            e.printStackTrace();
        }

        readInitialSettingsFromFile();

        frame.setVisible(true);
        frame.setSize(boardWidth, boardHeight);
        frame.setLocationRelativeTo(null);
        // frame.setResizable(false);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new BorderLayout());
        frame.setJMenuBar(menuBar);
        frame.add(boardPanel);

        menuBar.add(gameS);
        menuBar.add(helP);
        gameS.add(restart);
        gameS.add(boardEdit);
        gameS.add(scoreBoard);
        helP.add(howPlay);
        helP.add(about);
        boardEdit.add(size8x8);
        boardEdit.add(size16x16);
        boardEdit.add(size32x32);
        boardEdit.add(sizeCustom);
        boardPanel.setLayout(new GridLayout(numRows, numCols));

        size8x8.addActionListener(e -> setGameSize(8, 8, 10));
        size16x16.addActionListener(e -> setGameSize(16, 16, 40));
        size32x32.addActionListener(e -> setGameSize(30, 16, 99));
        sizeCustom.addActionListener(e -> showCustomSizeDialog());
        restart.addActionListener(e -> restartGame());
        scoreBoard.addActionListener(e -> showScoreboard());
        howPlay.addActionListener(e -> openHowToPlayURL());
        about.addActionListener(e -> openAboutFile());
        frame.add(textPanel, BorderLayout.NORTH);

        timer = new Timer(1000, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (gameStarted && !gameOver) {
                    long elapsedTime = (System.currentTimeMillis() - startTime) / 1000;
                    System.out.println("Elapsed Time: " + elapsedTime + " seconds");
                }
            }
        });

        startTime = System.currentTimeMillis(); // Add this line

        for (int r = 0; r < numRows; r++) {
            for (int c = 0; c < numCols; c++) {
                MineTile tile = new MineTile(r, c);
                board[r][c] = tile;

                tile.setFocusable(false);
                tile.setMargin(new Insets(0, 0, 1, 0));
                tile.setFont(new Font("pixelfont", Font.PLAIN, tileSize / 2));
                tile.setBackground(Color.DARK_GRAY);
                tile.setForeground(Color.LIGHT_GRAY);

                tile.setPreferredSize(new Dimension(tileSize, tileSize));

                tile.addMouseListener(new MouseAdapter() {
                    @Override
                    public void mousePressed(MouseEvent e) {
                        if (gameOver) {
                            return;
                        }
                        MineTile tile = (MineTile) e.getSource();

                        if (!timerStarted && e.getButton() == MouseEvent.BUTTON1) {
                            timer.start();
                            timerStarted = true;
                        }

                        if (e.getButton() == MouseEvent.BUTTON1) {
                            if (tile.getText() == "") {
                                if (mineList.contains(tile)) {
                                    revealMines();
                                } else {
                                    checkMine(tile.r, tile.c);
                                }
                            }
                        } else if (e.getButton() == MouseEvent.BUTTON3) {
                            if (tile.getText() == "" && tile.isEnabled()) {
                                tile.setText("{");
                            } else if (tile.getText() == "{") {
                                tile.setText("");
                            }
                        }
                    }
                });

                boardPanel.add(tile);
            }
        }

        frame.setVisible(true);

        setMines();
        frame.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                handleKeyPress(e);
            }
        });

    }

    void restartGame() {
        timer.stop();
        frame.dispose();

        if (minesweeperInstance != null) {
            if (gameOver && gameWon) {
                writeResultsToFile();
            }
            minesweeperInstance.initializeGame();
        } else {
            initializeGame();
        }
    }

    void setMines() {
        mineList = new ArrayList<MineTile>();

        int mineLeft = mineCount;
        while (mineLeft > 0) {
            int randomRow = random.nextInt(numRows);
            int randomCol = random.nextInt(numCols);

            MineTile tile = board[randomRow][randomCol];
            if (!mineList.contains(tile)) {
                mineList.add(tile);
                mineLeft -= 1;
            }
        }
    }

    void revealMines() {
        for (int i = 0; i < mineList.size(); i++) {
            MineTile tile = mineList.get(i);
            tile.setText("}");
        }

        gameOver = true;
        gameWon = false;
        JOptionPane.showMessageDialog(null, "Game Over");
    }

    void checkMine(int r, int c) {
        if (r < 0 || r >= numRows || c < 0 || c >= numCols) {
            return;
        }

        MineTile tile = board[r][c];
        if (!tile.isEnabled()) {
            return;
        }
        tile.setEnabled(false);
        tilesClicked += 1;

        int minesFound = 0;
        tile.setBackground(Color.WHITE);
        tile.setOpaque(true);
        minesFound += countMine(r - 1, c - 1);
        minesFound += countMine(r - 1, c);
        minesFound += countMine(r - 1, c + 1);

        minesFound += countMine(r, c - 1);
        minesFound += countMine(r, c + 1);

        minesFound += countMine(r + 1, c - 1);
        minesFound += countMine(r + 1, c);
        minesFound += countMine(r + 1, c + 1);

        if (minesFound > 0) {
            tile.setText(Integer.toString(minesFound));
            // System.out.println("Mines found: " + minesFound);

        } else {
            tile.setText("");

            tile.setBackground(Color.LIGHT_GRAY);
            tile.setOpaque(true);
            checkMine(r - 1, c - 1);
            checkMine(r - 1, c);
            checkMine(r - 1, c + 1);

            checkMine(r, c - 1);
            checkMine(r, c + 1);

            checkMine(r + 1, c - 1);
            checkMine(r + 1, c);
            checkMine(r + 1, c + 1);
        }

        if (tilesClicked == numRows * numCols - mineList.size()) {
            gameOver = true;
            gameWon = true;
            JOptionPane.showMessageDialog(null, "Mines Cleared");
        }
        lastClickedTile = board[r][c];
    }

    private void handleKeyPress(KeyEvent e) {
        if (e.getKeyChar() == 'b') {
            undoLastMove();
        }
    }

    void writeResultsToFile() {
        if (!gameWon) {
            return;
        }

        try (BufferedWriter writer = new BufferedWriter(new FileWriter(resultsFileName, true))) {
            long elapsedTime = (System.currentTimeMillis() - startTime) / 1000;
            String result = elapsedTime + " " + mineCount + "\n";
            writer.write(result);
        } catch (FileNotFoundException e) {
            System.err.println("Error: File not found - " + e.getMessage());
            JOptionPane.showMessageDialog(null, "Error: Results could not be saved. File not found.", "Error",
                    JOptionPane.ERROR_MESSAGE);
        } catch (IOException e) {
            e.printStackTrace();
            JOptionPane.showMessageDialog(null, "Error: Results could not be saved. IO Exception.", "Error",
                    JOptionPane.ERROR_MESSAGE);
        }
    }

    int countMine(int r, int c) {
        if (r < 0 || r >= numRows || c < 0 || c >= numCols) {
            return 0;
        }
        if (mineList.contains(board[r][c])) {
            return 1;
        }
        return 0;
    }

    void initializeGame() {
        timer.stop();

        if (gameOver) {
            if (gameWon) {
                writeResultsToFile();
            }
        }

        gameStarted = false;
        gameOver = false;
        gameWon = false;
        tilesClicked = 0;

        writeInitialSettingsToFile();

        new Minesweeper();
    }

    private void undoLastMove() {
        if (lastClickedTile != null) {

            lastClickedTile.setEnabled(true);
            lastClickedTile.setText("");
            lastClickedTile.setBackground(Color.DARK_GRAY);
            lastClickedTile.setOpaque(false);

            tilesClicked -= 1;
            lastClickedTile = null;

            if (gameOver) {
                gameOver = false;
                gameWon = false;
            }
        }
    }

    void showScoreboard() {
        JFrame scoreboardFrame = new JFrame("Scoreboard");
        scoreboardFrame.setSize(400, 300);
        scoreboardFrame.setLocationRelativeTo(null);
        scoreboardFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        JLabel titleLabel = new JLabel("Minesweeper Scoreboard");
        titleLabel.setFont(new Font("Arial", Font.BOLD, 14));
        titleLabel.setHorizontalAlignment(JLabel.CENTER);
        scoreboardFrame.add(titleLabel, BorderLayout.NORTH);

        JTextArea scoreboardTextArea = new JTextArea();
        scoreboardTextArea.setEditable(false);
        scoreboardTextArea.setFont(new Font("pixelfont", Font.PLAIN, 11));
        scoreboardTextArea.setMargin(new Insets(5, 0, 5, 0));

        try {
            File file = new File(resultsFileName);
            if (file.exists()) {
                Set<String> uniqueResults = new HashSet<>();
                java.util.Scanner scanner = new java.util.Scanner(file);
                while (scanner.hasNextLine()) {
                    uniqueResults.add(scanner.nextLine());
                }
                scanner.close();

                ArrayList<String> results = new ArrayList<>(uniqueResults);
                results.sort((result1, result2) -> {
                    String[] parts1 = result1.split(" ");
                    String[] parts2 = result2.split(" ");
                    long elapsedTime1 = Long.parseLong(parts1[0]);
                    int mineCount1 = Integer.parseInt(parts1[1]);
                    long elapsedTime2 = Long.parseLong(parts2[0]);
                    int mineCount2 = Integer.parseInt(parts2[1]);

                    double ratio1 = (double) elapsedTime1 / mineCount1;
                    double ratio2 = (double) elapsedTime2 / mineCount2;

                    return Double.compare(ratio1, ratio2);
                });

                for (int i = 0; i < results.size(); i++) {
                    String result = results.get(i);
                    String[] parts = result.split(" ");
                    long elapsedTime = Long.parseLong(parts[0]);
                    int mineCount = Integer.parseInt(parts[1]);

                    scoreboardTextArea.append("Rank " + (i + 1) + ": Time: " + elapsedTime +
                            " seconds, Mines: " + mineCount + "\n-------------------------\n");
                }
            } else {
                scoreboardTextArea.append("No results available.\n");
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            scoreboardTextArea.append("Error reading results.\n");
        }

        JScrollPane scrollPane = new JScrollPane(scoreboardTextArea);
        scoreboardFrame.add(scrollPane);

        scoreboardFrame.setBackground(Color.WHITE);
        scoreboardFrame.getRootPane().setBorder(BorderFactory.createLineBorder(Color.BLACK, 2));

        JButton closeButton = new JButton("Close");
        closeButton.addActionListener(e -> scoreboardFrame.dispose());
        scoreboardFrame.add(closeButton, BorderLayout.SOUTH);
        scoreboardFrame.setVisible(true);
    }

    private void writeInitialSettingsToFile() {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter("size.txt", false))) {
            writer.write(numRows + " " + numCols + " " + mineCount + "\n");
        } catch (IOException e) {
            e.printStackTrace();
            JOptionPane.showMessageDialog(null, "Error: Initial settings could not be saved. IO Exception.", "Error",
                    JOptionPane.ERROR_MESSAGE);
        }
    }

    private void readInitialSettingsFromFile() {
        try (Scanner scanner = new Scanner(new File("size.txt"))) {
            if (scanner.hasNextLine()) {
                String[] parts = scanner.nextLine().split(" ");
                numRows = Integer.parseInt(parts[0]);
                numCols = Integer.parseInt(parts[1]);
                mineCount = Integer.parseInt(parts[2]);
                boardWidth = numCols * tileSize;
                boardHeight = numRows * tileSize;
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            JOptionPane.showMessageDialog(null, "Error: Initial settings file not found.", "Error",
                    JOptionPane.ERROR_MESSAGE);
        }
    }

    private void showCustomSizeDialog() {
        JTextField rowsField = new JTextField();
        JTextField colsField = new JTextField();
        JTextField minesField = new JTextField();

        rowsField.setText(String.valueOf(numRows));
        colsField.setText(String.valueOf(numCols));
        minesField.setText(String.valueOf((int) (numCols * numRows * 0.16)));

        JPanel panel = new JPanel(new GridLayout(4, 2, 10, 10)); // Add horizontal and vertical gaps
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10)); // Add padding

        panel.add(new JLabel("Rows:", SwingConstants.RIGHT));
        panel.add(rowsField);
        panel.add(new JLabel("Columns:", SwingConstants.RIGHT));
        panel.add(colsField);
        panel.add(new JLabel("Mine Count (recommended):", SwingConstants.RIGHT));
        panel.add(minesField);

        JButton refreshButton = new JButton("Refresh");

        refreshButton.addActionListener(e -> {
            try {
                int customRows = Integer.parseInt(rowsField.getText());
                int customCols = Integer.parseInt(colsField.getText());

                int recommendedMines = (int) (customRows * customCols * 0.16);
                minesField.setText(String.valueOf(recommendedMines));
            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(null, "Invalid input. Please enter valid numbers.", "Error",
                        JOptionPane.ERROR_MESSAGE);
            }
        });

        panel.add(new JLabel());
        panel.add(refreshButton);

        int option = JOptionPane.showConfirmDialog(null, panel, "Custom Size", JOptionPane.OK_CANCEL_OPTION);

        if (option == JOptionPane.OK_OPTION) {
            try {
                int customRows = Integer.parseInt(rowsField.getText());
                int customCols = Integer.parseInt(colsField.getText());
                int customMines = Integer.parseInt(minesField.getText());

                if (customRows > 0 && customCols > 0 && customMines > 0 && customMines < customRows * customCols) {
                    numRows = customRows;
                    numCols = customCols;
                    mineCount = customMines;
                    restartGame();
                } else {
                    JOptionPane.showMessageDialog(null, "Invalid input. Please enter valid board size and mine count.",
                            "Error",
                            JOptionPane.ERROR_MESSAGE);
                }
            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(null, "Invalid input. Please enter valid numbers.", "Error",
                        JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void setGameSize(int rows, int cols, int mines) {
        numRows = rows;
        numCols = cols;
        mineCount = mines;
        restartGame();
    }

    private void openHowToPlayURL() {
        try {
            Desktop.getDesktop().browse(new URI("https://minesweepergame.com/strategy/how-to-play-minesweeper.php"));
        } catch (Exception ex) {
            ex.printStackTrace();
            JOptionPane.showMessageDialog(null, "Error opening the URL.", "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    private void openAboutFile() {
        try {
            File readmeFile = new File("readme.txt");

            if (!readmeFile.exists()) {
                try (BufferedWriter writer = new BufferedWriter(new FileWriter(readmeFile))) {
                    writer.write("Minesweeper game\n\n");
                    writer.write("Creator: ddanq4\n\n");
                    writer.write("You can use this product as you wish.\n\n");
                    writer.write("Ukraine, Kharkiv\n");
                    writer.write("December 2023\n");
                }
            }

            Desktop.getDesktop().edit(readmeFile);
        } catch (IOException ex) {
            ex.printStackTrace();
            JOptionPane.showMessageDialog(null, "Error opening or creating the readme file.", "Error",
                    JOptionPane.ERROR_MESSAGE);
        }
    }

}