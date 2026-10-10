import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import java.io.File;
import java.util.HashSet;
import java.util.Set;

public class national_groups_extractor {

    public static void main(String[] args) {
        try {
            File inputFile = new File("Popular_Baby_Names_NY.xml");
            SAXParserFactory factory = SAXParserFactory.newInstance();
            SAXParser saxParser = factory.newSAXParser();
            nationalgroupshandler handler = new nationalgroupshandler();
            saxParser.parse(inputFile, handler);

            System.out.println("National groups in the document:");
            for (String group : handler.getNationalGroups()) {
                System.out.println(group);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

class nationalgroupshandler extends DefaultHandler {

    private Set<String> nationalGroups = new HashSet<>();
    private boolean isEthnicity = false;

    public Set<String> getNationalGroups() {
        return nationalGroups;
    }

    @Override
    public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
        if (qName.equalsIgnoreCase("ethcty")) { // Изменено с "ethnicity" на "ethcty"
            isEthnicity = true;
        }
    }

    @Override
    public void characters(char ch[], int start, int length) throws SAXException {
        if (isEthnicity) {
            nationalGroups.add(new String(ch, start, length));
            isEthnicity = false;
        }
    }

    @Override
    public void endElement(String uri, String localName, String qName) throws SAXException {
        if (qName.equalsIgnoreCase("ethcty")) { // Изменено с "ethnicity" на "ethcty"
            isEthnicity = false;
        }
    }
}
