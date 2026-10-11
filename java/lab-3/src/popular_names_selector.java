import org.w3c.dom.*;
import javax.xml.parsers.*;
import javax.xml.transform.*;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import java.io.File;
import java.util.*;

class baby_name {
    String name;
    String gender;
    int count;
    int rank;

    baby_name(String name, String gender, int count, int rank) {
        this.name = name;
        this.gender = gender;
        this.count = count;
        this.rank = rank;
    }

    @Override
    public String toString() {
        return "Name: " + name + ", Gender: " + gender + ", Count: " + count + ", Rank: " + rank;
    }
}

public class popular_names_selector {

    public static void main(String[] args) {
        try {
            File inputFile = new File("Popular_Baby_Names_NY.xml");
            DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
            DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
            Document doc = dBuilder.parse(inputFile);
            doc.getDocumentElement().normalize();

            Set<String> ethnicities = new HashSet<>();
            NodeList rowList = doc.getElementsByTagName("row");

            for (int temp = 0; temp < rowList.getLength(); temp++) {
                Node rowNode = rowList.item(temp);

                if (rowNode.getNodeType() == Node.ELEMENT_NODE) {
                    Element rowElement = (Element) rowNode;
                    String ethnicity = rowElement.getElementsByTagName("ethcty").item(0).getTextContent();
                    ethnicities.add(ethnicity);
                }
            }

            List<String> ethnicityList = new ArrayList<>(ethnicities);
            Collections.sort(ethnicityList);

            Scanner scanner = new Scanner(System.in);
            System.out.println("Choose an ethnicity:");
            for (int i = 0; i < ethnicityList.size(); i++) {
                System.out.println((i + 1) + ". " + ethnicityList.get(i));
            }

            int choice = scanner.nextInt();
            String targetEthnicity = ethnicityList.get(choice - 1);

            int limit = 10;
            List<baby_name> babyNames = new ArrayList<>();
            Set<String> nameSet = new HashSet<>();

            for (int temp = 0; temp < rowList.getLength(); temp++) {
                Node rowNode = rowList.item(temp);

                if (rowNode.getNodeType() == Node.ELEMENT_NODE) {
                    Element rowElement = (Element) rowNode;

                    String ethnicity = rowElement.getElementsByTagName("ethcty").item(0).getTextContent();
                    if (ethnicity.equalsIgnoreCase(targetEthnicity)) {
                        String name = rowElement.getElementsByTagName("nm").item(0).getTextContent();
                        String gender = rowElement.getElementsByTagName("gndr").item(0).getTextContent();
                        int count = Integer.parseInt(rowElement.getElementsByTagName("cnt").item(0).getTextContent());
                        int rank = Integer.parseInt(rowElement.getElementsByTagName("rnk").item(0).getTextContent());

                        String uniqueKey = name + gender + count + rank;
                        if (!nameSet.contains(uniqueKey)) {
                            babyNames.add(new baby_name(name, gender, count, rank));
                            nameSet.add(uniqueKey);
                        }
                    }
                }
            }

            Collections.sort(babyNames, (b1, b2) -> b2.count - b1.count);

            Document newDoc = dBuilder.newDocument();
            Element rootElement = newDoc.createElement("baby_names");
            newDoc.appendChild(rootElement);

            for (int i = 0; i < limit && i < babyNames.size(); i++) {
                baby_name bn = babyNames.get(i);

                Element record = newDoc.createElement("record");

                Element name = newDoc.createElement("child_name");
                name.appendChild(newDoc.createTextNode(bn.name));
                record.appendChild(name);

                Element gender = newDoc.createElement("gender");
                gender.appendChild(newDoc.createTextNode(bn.gender));
                record.appendChild(gender);

                Element count = newDoc.createElement("count");
                count.appendChild(newDoc.createTextNode(String.valueOf(bn.count)));
                record.appendChild(count);

                Element rank = newDoc.createElement("rank");
                rank.appendChild(newDoc.createTextNode(String.valueOf(bn.rank)));
                record.appendChild(rank);

                rootElement.appendChild(record);
            }

            TransformerFactory transformerFactory = TransformerFactory.newInstance();
            Transformer transformer = transformerFactory.newTransformer();
            DOMSource domSource = new DOMSource(newDoc);
            StreamResult streamResult = new StreamResult(new File("popular_baby_names.xml"));
            transformer.transform(domSource, streamResult);

            System.out.println("New XML file created.");

            Document newDocRead = dBuilder.parse(new File("popular_baby_names.xml"));
            newDocRead.getDocumentElement().normalize();

            NodeList newNodeList = newDocRead.getElementsByTagName("record");

            for (int temp = 0; temp < newNodeList.getLength(); temp++) {
                Node node = newNodeList.item(temp);

                if (node.getNodeType() == Node.ELEMENT_NODE) {
                    Element element = (Element) node;

                    String name = element.getElementsByTagName("child_name").item(0).getTextContent();
                    String gender = element.getElementsByTagName("gender").item(0).getTextContent();
                    String count = element.getElementsByTagName("count").item(0).getTextContent();
                    String rank = element.getElementsByTagName("rank").item(0).getTextContent();

                    System.out
                            .println("Name: " + name + ", Gender: " + gender + ", Count: " + count + ", Rank: " + rank);
                }
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
