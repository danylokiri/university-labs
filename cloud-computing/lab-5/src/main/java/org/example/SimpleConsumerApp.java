package org.example;

import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.common.serialization.StringDeserializer;

import java.time.Duration;
import java.util.Collections;
import java.util.Properties;
import java.util.Scanner;

public class SimpleConsumerApp {

    public static void main(String[] args) {
        String topicName = "lab5-topic-1";

        Properties props = new Properties();
        props.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, "localhost:9092");
        props.put(ConsumerConfig.GROUP_ID_CONFIG, "lab5-simple-consumer-group");
        props.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class.getName());
        props.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class.getName());
        props.put(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG, "earliest");
        props.put(ConsumerConfig.MAX_POLL_RECORDS_CONFIG, "1");

        try (KafkaConsumer<String, String> consumer = new KafkaConsumer<>(props);
             Scanner scanner = new Scanner(System.in)) {

            consumer.subscribe(Collections.singletonList(topicName));
            System.out.println("Підписано на топік: " + topicName);
            System.out.println("Натисніть ENTER, щоб вичитати наступне повідомлення, або 'q' + ENTER для виходу.");

            while (true) {
                System.out.print("> ");
                String line = scanner.nextLine();
                if (line.equalsIgnoreCase("q")) {
                    System.out.println("Завершення SimpleConsumerApp.");
                    break;
                }

                ConsumerRecords<String, String> records =
                        consumer.poll(Duration.ofSeconds(5));

                if (records.isEmpty()) {
                    System.out.println("Немає нових повідомлень.");
                } else {
                    for (ConsumerRecord<String, String> record : records) {
                        System.out.printf(
                                "Отримано: value='%s' | topic=%s partition=%d offset=%d%n",
                                record.value(),
                                record.topic(),
                                record.partition(),
                                record.offset()
                        );
                    }
                }
            }
        }
    }
}
