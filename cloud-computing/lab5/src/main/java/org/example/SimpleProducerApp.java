package org.example;

import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.apache.kafka.clients.producer.RecordMetadata;
import org.apache.kafka.common.serialization.StringSerializer;

import java.util.Properties;
import java.util.Scanner;
import java.util.concurrent.Future;

public class SimpleProducerApp {

    public static void main(String[] args) throws Exception {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Введіть кількість повідомлень для надсилання: ");
        int count = scanner.nextInt();
        scanner.nextLine();

        String topicName = "lab5-topic-1";

        Properties props = new Properties();
        props.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, "localhost:9092");
        props.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, StringSerializer.class.getName());
        props.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, StringSerializer.class.getName());

        try (KafkaProducer<String, String> producer = new KafkaProducer<>(props)) {
            for (int i = 1; i <= count; i++) {
                String value = "Simple message #" + i;
                ProducerRecord<String, String> record =
                        new ProducerRecord<>(topicName, value);

                Future<RecordMetadata> future = producer.send(record);
                RecordMetadata metadata = future.get();

                System.out.printf(
                        "Надіслано: '%s' | topic=%s partition=%d offset=%d%n",
                        value,
                        metadata.topic(),
                        metadata.partition(),
                        metadata.offset()
                );
            }
        }

        System.out.println("Всі повідомлення надіслано. Завершення SimpleProducerApp.");
    }
}
