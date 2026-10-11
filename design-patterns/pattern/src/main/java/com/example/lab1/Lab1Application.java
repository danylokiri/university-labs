package com.example.lab1;

import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import java.util.Scanner;

@SpringBootApplication
public class Lab1Application implements CommandLineRunner {

    private final PowerOperation powerOperation; //in`ekcia zalezhnosti


    public Lab1Application(PowerOperation powerOperation) {
        this.powerOperation = powerOperation;
    }

    public static void main(String[] args) {
        SpringApplication app = new SpringApplication(Lab1Application.class);
        app.setAdditionalProfiles("import");
        app.run(args);
    }

    @Override
    public void run(String... args) {
        System.out.println("(0.-)");
        System.out.println("(-.o)");
        Scanner scanner = new Scanner(System.in);

        System.out.print("Введіть число a: ");
        int a = scanner.nextInt();

        System.out.print("Введіть число b (ступінь): ");
        int b = scanner.nextInt();

        long result = powerOperation.power(a, b);

        System.out.println(a + " ^ " + b + " = " + result);

        scanner.close();
    }
}
