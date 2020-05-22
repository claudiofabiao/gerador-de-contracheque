#include <stdio.h>
#include <locale.h>
#include <windows.h>

struct employee {
    int id;
    char name[80];
    char role[80];
};

struct fgts {
    float amount;
};

struct inss {
    float aliquot, amount;
};

struct irrf {
    int range;
    float aliquot, amount;
};

struct paycheck {
    char date[40];
    float grossAmount;
    float netAmount;

    struct employee employee;

    struct fgts fgts;
    struct inss inss;
    struct irrf irrf;
};

/**
 * Gets FGTS tax.
 */
struct fgts getFgts(float);

/**
 * Gets INSS tax (aliquot and final amount).
 */
struct inss getInss(float);

/**
 * Gets IRRF tax (range, aliquot and final amount).
 */
struct irrf getIrrf(float);

/**
 * Gets paycheck data from user input.
 */
struct paycheck getPaycheckData();

/**
 * Generates paycheck view in HTML file.
 */
void generate(struct paycheck);

/**
 * Current directory path.
 */
char currentDirectory[MAX_PATH];

int main() {
    char generateAgain;

    setlocale(LC_ALL, "");

    SetConsoleCP(1252);
    SetConsoleOutputCP(1252);

    GetCurrentDirectory(MAX_PATH, currentDirectory);

    printf("Gerador de Contracheque\n");
    printf("Versão: 1.0.0\n");

    do {
        generate(getPaycheckData());

        printf("Gerar outro contracheque?\n");
        printf("Insira \"s\" ou \"S\" para \"SIM\" ou qualquer outro caractere para \"NÃO\": ");

        scanf("%s", &generateAgain);
        fflush(stdin);
    } while(generateAgain == 's' || generateAgain == 'S');

    printf("Obrigado por utilizar o Gerador de Contracheque!");

    return 0;
}

/**
 * Gets FGTS tax.
 */
struct fgts getFgts(float amount) {
    struct fgts fgts;

    fgts.amount = amount * .08f;

    return fgts;
}

/**
 * Gets INSS tax (aliquot and final amount).
 */
struct inss getInss(float amount) {
    struct inss inss;

    if (amount <= 1045) {
        inss.aliquot = .075f;
        inss.amount  = amount * inss.aliquot;

    } else if (amount >= 1045.01 && amount <= 2089.6) {
        inss.aliquot = .09f;
        inss.amount  = amount * inss.aliquot - 15.67f;

    } else if (amount >= 2089.61 && amount <= 3134.4) {
        inss.aliquot = .12f;
        inss.amount  = amount * inss.aliquot - 78.36f;

    } else if (amount >= 3134.41 && amount <= 6101.06) {
        inss.aliquot = .14f;
        inss.amount  = amount * inss.aliquot - 141.05f;

    } else {
        inss.aliquot = .14f;
        inss.amount  = 713.1f;
    }

    return inss;
}

/**
 * Gets IRRF tax (range, aliquot and final amount).
 */
struct irrf getIrrf(float amount) {
    struct irrf irrf;

    if (amount <= 1903.98) {
        irrf.range   = 0;
        irrf.aliquot = .0f;
        irrf.amount  = .0f;

    } else if (amount >= 1903.99 && amount <= 2826.65) {
        irrf.range   = 1;
        irrf.aliquot = .075f;
        irrf.amount  = amount * irrf.aliquot - 142.8f;

    } else if (amount >= 2826.66 && amount <= 3751.05) {
        irrf.range   = 2;
        irrf.aliquot = .15f;
        irrf.amount  = amount * irrf.aliquot - 354.8f;

    } else if (amount >= 3751.06 && amount <= 4664.68) {
        irrf.range   = 3;
        irrf.aliquot = .225f;
        irrf.amount  = amount * irrf.aliquot - 636.13f;

    } else {
        irrf.range   = 4;
        irrf.aliquot = .275f;
        irrf.amount  = amount * irrf.aliquot - 869.36f;
    }

    return irrf;
}

/**
 * Gets paycheck data from user input.
 */
struct paycheck getPaycheckData() {
    struct paycheck paycheck;

    printf("\nData de referência (mês e ano): ");
    gets(paycheck.date);

    printf("\nCódigo do funcionário: ");
    scanf("%d", &paycheck.employee.id);
    fflush(stdin);

    printf("Nome do funcionário: ");
    gets(paycheck.employee.name);

    printf("Cargo do funcionário: ");
    gets(paycheck.employee.role);

    printf("\nSalário bruto: ");
    scanf("%f", &paycheck.grossAmount);
    fflush(stdin);

    paycheck.fgts = getFgts(paycheck.grossAmount);
    paycheck.inss = getInss(paycheck.grossAmount);
    paycheck.irrf = getIrrf(paycheck.grossAmount - paycheck.inss.amount);

    paycheck.netAmount = paycheck.grossAmount - paycheck.inss.amount - paycheck.irrf.amount;

    return paycheck;
}

/**
 * Generates paycheck view in HTML file.
 */
void generate(struct paycheck paycheck) {
    char documentTitle[sizeof(paycheck.date) + sizeof(paycheck.employee.id) + sizeof(paycheck.employee.name) + 15];
    char fileName[sizeof(documentTitle) + 5];
    FILE *file;

    sprintf(documentTitle, "Contracheque_%s_%d_%s", paycheck.date, paycheck.employee.id, paycheck.employee.name);
    sprintf(fileName, "%s.html", documentTitle);

    file = fopen(fileName, "w");

    fprintf(file, "<!DOCTYPE html>"
                  "<html lang=\"pt-br\">"
                  "<head>"
                  "    <meta charset=\"windows-1252\">"
                  "    <title>%s</title>"
                  "    <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css\" integrity=\"sha384-9aIt2nRpC12Uk9gS9baDl411NQApFmC26EwAOH8WgZl5MYYxFfc+NcPb1dKGj7Sk\" crossorigin=\"anonymous\">"
                  "    <style>"
                  "        @media (min-width: 1200px) {"
                  "            .container {"
                  "                max-width: 960px;"
                  "            }"
                  "        }"
                  "        .document-info .document-title {"
                  "            font-size: 2rem;"
                  "        }"
                  "        .document-info .company-name {"
                  "            font-size: 1.5rem;"
                  "        }"
                  "        .document-info .company-identifier {"
                  "            font-size: 1.25rem;"
                  "        }"
                  "        .document-info .document-date {"
                  "            font-size: 1rem;"
                  "        }"
                  "    </style>"
                  "</head>"
                  "<body>"
                  "<main>"
                  "    <section class=\"document-info\">"
                  "        <div class=\"container py-5 border-bottom\">"
                  "            <div class=\"row\">"
                  "                <div class=\"col\">"
                  "                    <h1 class=\"document-title\">Demonstrativo de pagamento de salário</h1>"
                  "                    <h2 class=\"company-name\">Empresa de Exemplo Ltda.</h2>"
                  "                    <h3 class=\"company-identifier\">00.000.000/0000-00</h3>"
                  "                    <h4 class=\"document-date\">Data de referência: %s</h4>"
                  "                </div>"
                  "            </div>"
                  "        </div>"
                  "    </section>"
                  "    <section class=\"employee-info\">"
                  "        <div class=\"container py-5 border-bottom\">"
                  "            <div class=\"row\">"
                  "                <div class=\"col\">"
                  "                    <h5 class=\"mb-3\">Identificação do funcionário</h5>"
                  "                    <table class=\"table mb-0\">"
                  "                        <thead class=\"thead-light\">"
                  "                        <tr>"
                  "                            <th>Código</th>"
                  "                            <th>Nome</th>"
                  "                            <th>Cargo</th>"
                  "                        </tr>"
                  "                        </thead>"
                  "                        <tbody>"
                  "                        <tr>"
                  "                            <td>%d</td>"
                  "                            <td>%s</td>"
                  "                            <td>%s</td>"
                  "                        </tr>"
                  "                        </tbody>"
                  "                    </table>"
                  "                </div>"
                  "            </div>"
                  "        </div>"
                  "    </section>"
                  "    <section class=\"payment-info\">"
                  "        <div class=\"container py-5 border-bottom\">"
                  "            <div class=\"row\">"
                  "                <div class=\"col\">"
                  "                    <h5 class=\"mb-3\">Lançamentos</h5>"
                  "                    <table class=\"table mb-0\">"
                  "                        <thead class=\"thead-light\">"
                  "                        <tr>"
                  "                            <th>Descrição</th>"
                  "                            <th>Vencimento</th>"
                  "                            <th>Desconto</th>"
                  "                        </tr>"
                  "                        </thead>"
                  "                        <tbody>"
                  "                        <tr>"
                  "                            <td>Salário</td>"
                  "                            <td>%0.2f</td>"
                  "                            <td></td>"
                  "                        </tr>"
                  "                        <tr>"
                  "                            <td>INSS</td>"
                  "                            <td></td>"
                  "                            <td>%0.2f</td>"
                  "                        </tr>"
                  "                        <tr>"
                  "                            <td>IRRF</td>"
                  "                            <td></td>"
                  "                            <td>%0.2f</td>"
                  "                        </tr>"
                  "                        </tbody>"
                  "                        <tfoot>"
                  "                        <tr>"
                  "                            <td class=\"font-weight-bold\">Totais</td>"
                  "                            <td>%0.2f</td>"
                  "                            <td>%0.2f</td>"
                  "                        </tr>"
                  "                        <tr class=\"table-active\">"
                  "                            <td class=\"font-weight-bold\">Salário líquido</td>"
                  "                            <td class=\"font-weight-bold\" colspan=\"2\">%0.2f</td>"
                  "                        </tr>"
                  "                        </tfoot>"
                  "                    </table>"
                  "                </div>"
                  "            </div>"
                  "        </div>"
                  "    </section>"
                  "    <section class=\"additional-info\">"
                  "        <div class=\"container py-5\">"
                  "            <div class=\"row\">"
                  "                <div class=\"col\">"
                  "                    <h5 class=\"mb-3\">Informações adicionais</h5>"
                  "                    <table class=\"table mb-0\">"
                  "                        <thead class=\"thead-light\">"
                  "                        <tr>"
                  "                            <th>FGTS</th>"
                  "                            <th>Base de cálculo do INSS</th>"
                  "                            <th>Alíquota do INSS</th>"
                  "                            <th>Base de cálculo do IRRF</th>"
                  "                            <th>Faixa do IRRF</th>"
                  "                            <th>Alíquota do IRRF</th>"
                  "                        </tr>"
                  "                        </thead>"
                  "                        <tbody>"
                  "                        <tr>"
                  "                            <td>%0.2f</td>"
                  "                            <td>%0.2f</td>"
                  "                            <td>%0.2f%%</td>"
                  "                            <td>%0.2f</td>"
                  "                            <td>%d</td>"
                  "                            <td>%0.2f%%</td>"
                  "                        </tr>"
                  "                        </tbody>"
                  "                    </table>"
                  "                </div>"
                  "            </div>"
                  "        </div>"
                  "    </section>"
                  "</main>"
                  "</body>"
                  "</html>", documentTitle, paycheck.date, paycheck.employee.id, paycheck.employee.name, paycheck.employee.role, paycheck.grossAmount, paycheck.inss.amount, paycheck.irrf.amount, paycheck.grossAmount, paycheck.inss.amount + paycheck.irrf.amount, paycheck.netAmount, paycheck.fgts.amount, paycheck.grossAmount, paycheck.inss.aliquot * 100, paycheck.grossAmount - paycheck.inss.amount, paycheck.irrf.range, paycheck.irrf.aliquot * 100);

    fclose(file);

    printf("\nContracheque gerado com sucesso.\n");
    printf("Arquivo salvo em %s\\%s.\n\n", currentDirectory, fileName);

    ShellExecute(NULL, "open", fileName, NULL, NULL, 1);
}