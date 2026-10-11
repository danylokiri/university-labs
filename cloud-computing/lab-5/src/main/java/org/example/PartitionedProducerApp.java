package org.example;

import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.apache.kafka.clients.producer.RecordMetadata;
import org.apache.kafka.common.serialization.StringSerializer;

import java.util.Properties;
import java.util.Scanner;
import java.util.concurrent.Future;

public class PartitionedProducerApp {

    public static void main(String[] args) throws Exception {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Введіть кількість повідомлень для надсилання (завдання 2): ");
        int count = scanner.nextInt();
        scanner.nextLine();

        String topicName = "lab5-topic-2";

        Properties props = new Properties();
        props.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, "localhost:9092");
        props.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, StringSerializer.class.getName());
        props.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, StringSerializer.class.getName());

        try (KafkaProducer<String, String> producer = new KafkaProducer<>(props)) {
            for (int i = 1; i <= count; i++) {
                String key = "key-" + (i % 3);
                String value = "Partitioned message #" + i + " (key=" + key + ")";

                ProducerRecord<String, String> record =
                        new ProducerRecord<>(topicName, key, value);

                Future<RecordMetadata> future = producer.send(record);
                RecordMetadata metadata = future.get();

                System.out.printf(
                        "Надіслано: '%s' | key=%s topic=%s partition=%d offset=%d%n",
                        value,
                        key,
                        metadata.topic(),
                        metadata.partition(),
                        metadata.offset()
                );
            }
        }

        System.out.println("Всі повідомлення для топіка з 3 партиціями надіслані. Завершення PartitionedProducerApp.");
    }
}
