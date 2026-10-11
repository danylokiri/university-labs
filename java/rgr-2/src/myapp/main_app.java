package myapp;

import mycomponents.data_table;
import mycomponents.data_chart;

import javax.swing.*;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.DefaultTableModel;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.helpers.DefaultHandler;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.List;

public class main_app extends JFrame {
    private data_table tableComponent;
    private data_chart chartComponent;
    private JButton openButton;
    private JButton saveButton;
    private JButton clearButton;
    private JButton exitButton;

    public main_app() {
        setTitle("Приклад JavaBeans");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        tableComponent = new data_table();
        chartComponent = new data_chart();

        JPanel leftPanel = new JPanel();
        leftPanel.setLayout(new BorderLayout());

        leftPanel.add(tableComponent, BorderLayout.CENTER);

        add(leftPanel, BorderLayout.WEST);
        add(chartComponent, BorderLayout.CENTER);

        JPanel controlPanel = new JPanel();
        openButton = new JButton("Відкрити");
        saveButton = new JButton("Зберегти");
        clearButton = new JButton("Очистити");
        exitButton = new JButton("Завершити");

        controlPanel.add(openButton);
        controlPanel.add(saveButton);
        controlPanel.add(clearButton);
        controlPanel.add(exitButton);

        add(controlPanel, BorderLayout.SOUTH);

        openButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                openFile();
            }
        });

        saveButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                saveFile();
            }
        });

        clearButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                tableComponent.getTableModel().setRowCount(0);
                updateChart();
            }
        });

        exitButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.exit(0);
            }
        });

        tableComponent.getTableModel().addTableModelListener(new TableModelListener() {
            @Override
            public void tableChanged(TableModelEvent e) {
                updateChart();
            }
        });

        pack();
        setLocationRelativeTo(null);
        setVisible(true);
    }

    private void openFile() {
        JFileChooser fileChooser = new JFileChooser();
        int result = fileChooser.showOpenDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            loadDataFromFile(file);
        }
    }

    private void saveFile() {
        JFileChooser fileChooser = new JFileChooser();
        int result = fileChooser.showSaveDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            saveDataToFile(file);
        }
    }

    private void loadDataFromFile(File file) {
        try {
            SAXParserFactory factory = SAXParserFactory.newInstance();
            SAXParser parser = factory.newSAXParser();
            DefaultHandler handler = new DefaultHandler() {
                List<Point> points = new ArrayList<>();
                DefaultTableModel model = tableComponent.getTableModel();

                @Override
                public void startDocument() {
                    model.setRowCount(0);
                }

                @Override
                public void startElement(String uri, String localName, String qName, Attributes attributes) {
                    if (qName.equals("entry")) {
                        String date = attributes.getValue("date");
                        int x = Integer.parseInt(attributes.getValue("x"));
                        int y = Integer.parseInt(attributes.getValue("y"));
                        model.addRow(new Object[] { date, x, y });
                        points.add(new Point(x, y));
                    }
                }

                @Override
                public void endDocument() {
                    chartComponent.setPoints(points);
                }
            };
            parser.parse(new InputSource(new FileInputStream(file)), handler);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void saveDataToFile(File file) {
        try {
            DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
            DocumentBuilder docBuilder = docFactory.newDocumentBuilder();
            Document doc = docBuilder.newDocument();
            Element rootElement = doc.createElement("data");
            doc.appendChild(rootElement);

            DefaultTableModel model = tableComponent.getTableModel();
            for (int i = 0; i < model.getRowCount(); i++) {
                Element entry = doc.createElement("entry");
                entry.setAttribute("date", model.getValueAt(i, 0).toString());
                entry.setAttribute("x", model.getValueAt(i, 1).toString());
                entry.setAttribute("y", model.getValueAt(i, 2).toString());
                rootElement.appendChild(entry);
            }

            TransformerFactory transformerFactory = TransformerFactory.newInstance();
            Transformer transformer = transformerFactory.newTransformer();
            DOMSource source = new DOMSource(doc);
            StreamResult result = new StreamResult(new FileOutputStream(file));
            transformer.transform(source, result);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void updateChart() {
        DefaultTableModel model = tableComponent.getTableModel();
        List<Point> points = new ArrayList<>();
        for (int i = 0; i < model.getRowCount(); i++) {
            int x = (int) model.getValueAt(i, 1);
            int y = (int) model.getValueAt(i, 2);
            points.add(new Point(x, y));
        }
        chartComponent.setPoints(points);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new main_app();
            }
        });
    }
}
