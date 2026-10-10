import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import java.io.File;
import java.util.HashSet;
import java.util.Set;

public class zavdannya0 {

    public static void main(String[] args) {
        try {
            File inputFile = new File("Popular_Baby_Names_NY.xml");
            SAXParserFactory factory = SAXParserFactory.newInstance();
            SAXParser saxParser = factory.newSAXParser();
            userhandler userhandler = new userhandler();
            saxParser.parse(inputFile, userhandler);

            System.out.println("Tags in the document:");
            for (String tag : userhandler.getTags()) {
                System.out.println(tag);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

class userhandler extends DefaultHandler {

    private Set<String> tags = new HashSet<>();

    public Set<String> getTags() {
        return tags;
    }

    @Override
    public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
        tags.add(qName);
    }

    @Override
    public void characters(char ch[], int start, int length) throws SAXException {
        // This method can be used to extract content if needed
    }

    @Override
    public void endElement(String uri, String localName, String qName) throws SAXException {
        // This method can be used to handle end of elements if needed
    }
}
