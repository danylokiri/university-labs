import java.util.ArrayList;
import java.util.List;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;
import javax.swing.JFrame;

public class DoubleSpendAttack {
    public static void main(String[] args) {
        double T = 30;
        double alpha1 = 0.00167;
        double alpha2 = 1.0 / T;

        double[] pMValues = { 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4 };
        int[] dHValues = { 0, 15, 30, 60, 120, 180 };

        List<DataPoint> dataPointsAlpha1 = new ArrayList<>();
        List<DataPoint> dataPointsAlpha2 = new ArrayList<>();

        System.out.println("Calculations for alpha1 = " + alpha1);
        performCalculations(alpha1, pMValues, dHValues, dataPointsAlpha1);
        System.out.println("Calculations for alpha2 = " + alpha2);
        performCalculations(alpha2, pMValues, dHValues, dataPointsAlpha2);

        plotCombinedGraph(dataPointsAlpha1, dataPointsAlpha2, alpha1, alpha2);
    }

    private static void performCalculations(double alpha, double[] pMValues, int[] dHValues,
            List<DataPoint> dataPoints) {
        for (double pM : pMValues) {
            double pH = 1 - pM;
            double alphaH = alpha * pH;
            double alphaM = alpha * pM;
            double alphaSum = alphaH + alphaM;

            // Использование формулы (1)
            pH = alphaH / alphaSum;
            pM = alphaM / alphaSum;

            for (int dH : dHValues) {
                // Использование формулы (2)
                double pHPrime = pH * Math.exp(-alphaM * dH); // Обновлено для корректного следования формуле (2)
                double pMPrime = 1 - pHPrime; // Убедились, что вычисление pMPrime соответствует pHPrime

                int z = findMinimumConfirmations(alphaH, alphaM, pMPrime, pHPrime);

                dataPoints.add(new DataPoint(pM, dH, z));
                System.out.println(String.format(
                        "Для alpha = %.10f, pM = %.2f, dH = %d мiнiмальна кiлькiсть блокiв пiдтвердження: %d", alpha,
                        pM, dH, z));
            }
        }
    }

    private static int findMinimumConfirmations(double alphaH, double alphaM, double pMPrime, double pHPrime) {
        int z = 1;
        int maxIterations = 10000; // Ограничение количества итераций для предотвращения бесконечного цикла
        while (z <= maxIterations) {
            double attackProbability = calculateAttackProbability(alphaH, alphaM, pMPrime, pHPrime, z);
            if (attackProbability < 0.001) {
                break;
            }
            z++;
        }
        if (z > maxIterations) {
            System.out.println(
                    "Превышено максимальное количество итераций при расчете минимального количества блоков подтверждения");
        }
        return z;
    }

    // Использование формулы (3)
    private static double calculateAttackProbability(double alphaH, double alphaM, double pMPrime, double pHPrime,
            int z) {
        // Проверка корректности переданных значений alphaH и alphaM
        if (alphaH < 0 || alphaM < 0) {
            throw new IllegalArgumentException("Значения alphaH и alphaM должны быть неотрицательными.");
        }

        if (pMPrime >= pHPrime) {
            return 1.0;
        }

        double sum = 0.0;
        for (int k = 0; k <= z; k++) {
            double binomialCoefficient = Math.exp(calculateLogBinomialCoefficient(z, k)); // Используем exp() для
                                                                                          // получения биномиального
                                                                                          // коэффициента
            double probabilityTerm = Math.pow(pMPrime / pHPrime, k);
            double term = binomialCoefficient * probabilityTerm * Math.pow(1 - (pMPrime / pHPrime), z - k);
            sum += term;
        }

        // Проверка корректности вычисления суммы
        if (sum > 1.0) {
            sum = 1.0; // Ограничение суммы, чтобы избежать ошибок округления, приводящих к значениям >
                       // 1
        }

        return Math.max(0, 1 - sum); // Ограничение вероятности на случай ошибок округления
    }

    private static double calculateLogBinomialCoefficient(int n, int k) {
        if (k == 0 || k == n) {
            return 0.0; // Логарифм биномиального коэффициента для k == 0 или k == n равен 0 (так как
                        // exp(0) = 1)
        }
        double logResult = 0.0;
        for (int i = 1; i <= k; i++) {
            logResult += Math.log(n - i + 1) - Math.log(i);
        }
        return logResult; // Возвращаем логарифм результата, чтобы избежать проблем с переполнением
    }

    private static void plotCombinedGraph(List<DataPoint> dataPointsAlpha1, List<DataPoint> dataPointsAlpha2,
            double alpha1, double alpha2) {
        XYSeriesCollection dataset = new XYSeriesCollection();

        XYSeries seriesAlpha1 = new XYSeries("Alpha = " + alpha1);
        for (DataPoint point : dataPointsAlpha1) {
            seriesAlpha1.add(point.pM, point.z);
        }
        dataset.addSeries(seriesAlpha1);

        XYSeries seriesAlpha2 = new XYSeries("Alpha = " + alpha2);
        for (DataPoint point : dataPointsAlpha2) {
            seriesAlpha2.add(point.pM, point.z);
        }
        dataset.addSeries(seriesAlpha2);

        JFreeChart chart = ChartFactory.createXYLineChart(
                "Залежність кількості блоків підтвердження від pM",
                "Частка зловмисників (pM)",
                "Кількість блоків підтвердження (z)",
                dataset,
                PlotOrientation.VERTICAL,
                true,
                true,
                false);

        JFrame frame = new JFrame("Залежність кількості блоків підтвердження від pM для різних alpha");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().add(new ChartPanel(chart));
        frame.pack();
        frame.setVisible(true);
    }

    static class DataPoint {
        double pM;
        int dH;
        int z;

        DataPoint(double pM, int dH, int z) {
            this.pM = pM;
            this.dH = dH;
            this.z = z;
        }
    }
}
