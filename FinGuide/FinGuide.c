#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Product {
    char name[100];
    char bank[100];
    double afterTaxInterestRate;
    int period;
    char restrictions[256];
    struct Product* next;
} Product;

Product* depositHead = NULL;
Product* savingHead = NULL;

void mainMenu();
void HMain(int choice);
void AddDepositInfo();
void AddSavingInfo();
void RemoveDepositInfo();
void RemoveSavingInfo();
void RecommendProduct(Product* head, double amount, int period, const char* bank, int age);
void AddProduct(Product** head);
void RemoveProduct(Product** head);
void SearchProduct(Product* head);
void SaveProducts(Product* head, const char* filename);
void LoadProducts(Product** head, const char* filename);
int CheckRestrictions(const char* restrictions, double amount, int period, const char* bank, int age);

int main() {
    LoadProducts(&depositHead, "depositProducts.txt");
    LoadProducts(&savingHead, "savingProducts.txt");

    int choice;
    while (1) {
        mainMenu();
        printf("�޴��� �����ϼ���: ");
        scanf("%d", &choice);
        HMain(choice);
    }

    return 0;
}

void mainMenu() {
    printf("\n===== ���� �޴� =====\n");
    printf("1. ���� ��ǰ ��õ\n");
    printf("2. ���� ��ǰ ��õ\n");
    printf("3. ���� ��ǰ �߰�\n");
    printf("4. ���� ��ǰ �߰�\n");
    printf("5. ���� ��ǰ ����\n");
    printf("6. ���� ��ǰ ����\n");
    printf("7. ��ǰ �˻�\n");
    printf("8. ���α׷� ����\n");
    printf("=====================\n");
}

void HMain(int choice) {
    double amount;
    int period;
    char bank[100];
    int age;

    switch (choice) {
    case 1:
    case 2:
        printf("���� �ݾ��� �Է��ϼ���: ");
        scanf("%lf", &amount);
        printf("���� �Ⱓ�� �Է��ϼ���(����): ");
        scanf("%d", &period);
        printf("�� �̿� ������ �Է��ϼ���: ");
        getchar();
        fgets(bank, sizeof(bank), stdin);
        bank[strcspn(bank, "\n")] = '\0';
        printf("���̸� �Է��ϼ���: ");
        scanf("%d", &age);

        if (choice == 1) {
            RecommendProduct(depositHead, amount, period, bank, age);
        }
        else {
            RecommendProduct(savingHead, amount, period, bank, age);
        }
        break;
    case 3:
        AddProduct(&depositHead);
        SaveProducts(depositHead, "depositProducts.txt");
        break;
    case 4:
        AddProduct(&savingHead);
        SaveProducts(savingHead, "savingProducts.txt");
        break;
    case 5:
        RemoveProduct(&depositHead);
        SaveProducts(depositHead, "depositProducts.txt");
        break;
    case 6:
        RemoveProduct(&savingHead);
        SaveProducts(savingHead, "savingProducts.txt");
        break;
    case 7:
        printf("\n===== ��ǰ �˻� =====\n");
        printf("1. ���� ��ǰ �˻�\n");
        printf("2. ���� ��ǰ �˻�\n");
        printf("=====================\n");
        printf("�˻��� ī�װ��� �����ϼ���: ");
        scanf("%d", &choice);
        if (choice == 1) {
            SearchProduct(depositHead);
        }
        else if (choice == 2) {
            SearchProduct(savingHead);
        }
        else {
            printf("�߸��� �����Դϴ�. �ٽ� �������ּ���.\n");
        }
        break;
    case 8:
        printf("���α׷��� �����մϴ�.\n");
        SaveProducts(depositHead, "depositProducts.txt");
        SaveProducts(savingHead, "savingProducts.txt");
        exit(0);
    default:
        printf("�߸��� �����Դϴ�. �ٽ� �������ּ���.\n");
    }
}

void AddProduct(Product** head) {
    Product* newProduct = (Product*)malloc(sizeof(Product));
    if (newProduct == NULL) {
        printf("�޸� �Ҵ翡 �����߽��ϴ�.\n");
        return;
    }

    printf("��ǰ �̸��� �Է��ϼ���: ");
    getchar();
    fgets(newProduct->name, sizeof(newProduct->name), stdin);
    newProduct->name[strcspn(newProduct->name, "\n")] = '\0';
    printf("���� �̸��� �Է��ϼ���: ");
    fgets(newProduct->bank, sizeof(newProduct->bank), stdin);
    newProduct->bank[strcspn(newProduct->bank, "\n")] = '\0';
    printf("���� �������� �Է��ϼ���: ");
    scanf("%lf", &newProduct->afterTaxInterestRate);
    printf("�Ⱓ(����)�� �Է��ϼ���: ");
    scanf("%d", &newProduct->period);
    printf("���� ����(��: ����<30 �Ǵ� 20<����<30)�� �Է��ϼ���: ");
    getchar();
    fgets(newProduct->restrictions, sizeof(newProduct->restrictions), stdin);
    newProduct->restrictions[strcspn(newProduct->restrictions, "\n")] = '\0';

    newProduct->next = *head;
    *head = newProduct;
    printf("��ǰ�� �߰��Ǿ����ϴ�.\n");
}

void RemoveProduct(Product** head) {
    if (*head == NULL) {
        printf("������ ��ǰ�� �����ϴ�.\n");
        return;
    }

    char productName[100];
    int productPeriod;
    printf("������ ��ǰ �̸��� �Է��ϼ���: ");
    getchar();
    fgets(productName, sizeof(productName), stdin);
    productName[strcspn(productName, "\n")] = '\0';
    printf("������ ��ǰ�� �Ⱓ(����)�� �Է��ϼ���: ");
    scanf("%d", &productPeriod);

    Product* current = *head;
    Product* prev = NULL;

    while (current != NULL && (strcmp(current->name, productName) != 0 || current->period != productPeriod)) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("�ش� �̸��� �Ⱓ�� ��ǰ�� ã�� �� �����ϴ�.\n");
        return;
    }

    if (prev == NULL) {
        *head = current->next;
    }
    else {
        prev->next = current->next;
    }

    free(current);
    printf("��ǰ�� �����Ǿ����ϴ�.\n");
}

void SearchProduct(Product* head) {
    if (head == NULL) {
        printf("�˻��� ��ǰ�� �����ϴ�.\n");
        return;
    }

    char productName[100];
    char bankName[100];
    int productPeriod;
    printf("�˻��� ��ǰ �̸��� �Է��ϼ���: ");
    getchar();
    fgets(productName, sizeof(productName), stdin);
    productName[strcspn(productName, "\n")] = '\0';
    printf("�˻��� ���� �̸��� �Է��ϼ���: ");
    fgets(bankName, sizeof(bankName), stdin);
    bankName[strcspn(bankName, "\n")] = '\0';
    printf("�˻��� ��ǰ�� �Ⱓ(����)�� �Է��ϼ���: ");
    scanf("%d", &productPeriod);

    Product* current = head;
    while (current != NULL) {
        if (strcmp(current->name, productName) == 0 && strcmp(current->bank, bankName) == 0 && current->period == productPeriod) {
            printf("\n��ǰ ������ ã�ҽ��ϴ�:\n");
            printf("��ǰ �̸�: %s\n", current->name);
            printf("����: %s\n", current->bank);
            printf("���� ������: %.2f%%\n", current->afterTaxInterestRate);
            printf("�Ⱓ: %d����\n", current->period);
            printf("���� ����: %s\n", current->restrictions);
            return;
        }
        current = current->next;
    }

    printf("�ش� ���ǿ� �´� ��ǰ�� ã�� �� �����ϴ�.\n");
}

void RecommendProduct(Product* head, double amount, int period, const char* bank, int age) {
    if (head == NULL) {
        printf("��õ�� ��ǰ�� �����ϴ�.\n");
        return;
    }

    Product* bestProduct = NULL;
    Product* bestBankProduct = NULL;
    double bestProductReturn = 0.0;
    double bestBankProductReturn = 0.0;

    for (Product* p = head; p != NULL; p = p->next) {
        if (CheckRestrictions(p->restrictions, amount, period, bank, age) && p->period == period) {
            double productReturn = amount * (1 + (p->afterTaxInterestRate / 100) * (period / 12.0));
            if (bestProduct == NULL || productReturn > bestProductReturn) {
                bestProduct = p;
                bestProductReturn = productReturn;
            }
            if (strcmp(p->bank, bank) == 0) {
                double bankProductReturn = amount * (1 + (p->afterTaxInterestRate / 100) * (period / 12.0));
                if (bestBankProduct == NULL || bankProductReturn > bestBankProductReturn) {
                    bestBankProduct = p;
                    bestBankProductReturn = bankProductReturn;
                }
            }
        }
    }

    if (bestProduct != NULL) {
        printf("\n��ü ��ǰ �� ��õ ��ǰ:\n");
        printf("��ǰ �̸�: %s\n", bestProduct->name);
        printf("����: %s\n", bestProduct->bank);
        printf("���� ������: %.2f%%\n", bestProduct->afterTaxInterestRate);
        printf("�Ⱓ: %d����\n", bestProduct->period);
        printf("���� ����: %s\n", bestProduct->restrictions);
        printf("���� ���ɾ�: %d��\n", (int)bestProductReturn);
    }
    else {
        printf("��ü ��ǰ �� ���ǿ� �´� ��õ ��ǰ�� �����ϴ�.\n");
    }

    if (bestBankProduct != NULL) {
        printf("\n�� �̿� ���� �� ��õ ��ǰ:\n");
        printf("��ǰ �̸�: %s\n", bestBankProduct->name);
        printf("����: %s\n", bestBankProduct->bank);
        printf("���� ������: %.2f%%\n", bestBankProduct->afterTaxInterestRate);
        printf("�Ⱓ: %d����\n", bestBankProduct->period);
        printf("���� ����: %s\n", bestBankProduct->restrictions);
        printf("���� ���ɾ�: %d��\n", (int)bestBankProductReturn);
    }
    else {
        printf("�� �̿� ���� �� ���ǿ� �´� ��õ ��ǰ�� �����ϴ�.\n");
    }
}

int CheckRestrictions(const char* restrictions, double amount, int period, const char* bank, int age) {
    int minAge = 0, maxAge = 0;

    if (strstr(restrictions, "����<") != NULL && sscanf(restrictions, "����<%d", &maxAge) == 1) {
        if (age >= maxAge) return 0;
    }
    else if (strstr(restrictions, "����>") != NULL && sscanf(restrictions, "����>%d", &minAge) == 1) {
        if (age <= minAge) return 0;
    }
    else if (strstr(restrictions, "����=") != NULL && sscanf(restrictions, "����=%d", &minAge) == 1) {
        if (age != minAge) return 0;
    }
    else if (strstr(restrictions, "����<=") != NULL && sscanf(restrictions, "����<=%d", &maxAge) == 1) {
        if (age > maxAge) return 0;
    }
    else if (strstr(restrictions, "����>=") != NULL && sscanf(restrictions, "����>=%d", &minAge) == 1) {
        if (age < minAge) return 0;
    }
    else if (sscanf(restrictions, "%d<=����<=%d", &minAge, &maxAge) == 2) {
        if (age < minAge || age > maxAge) return 0;
    }

    return 1;
}

void SaveProducts(Product* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("������ ������ �� �����ϴ�: %s\n", filename);
        return;
    }

    Product* current = head;
    while (current != NULL) {
        fprintf(file, "%s,%s,%.2f,%d,%s\n",
            current->name, current->bank, current->afterTaxInterestRate,
            current->period, current->restrictions);
        current = current->next;
    }

    fclose(file);
}

void LoadProducts(Product** head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("������ �� �� �����ϴ�: %s\n", filename);
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        Product* newProduct = (Product*)malloc(sizeof(Product));
        if (newProduct == NULL) {
            printf("�޸� �Ҵ翡 �����߽��ϴ�.\n");
            fclose(file);
            return;
        }

        char* token = strtok(line, ",");
        strcpy(newProduct->name, token);

        token = strtok(NULL, ",");
        strcpy(newProduct->bank, token);

        token = strtok(NULL, ",");
        newProduct->afterTaxInterestRate = atof(token);

        token = strtok(NULL, ",");
        newProduct->period = atoi(token);

        token = strtok(NULL, ",");
        strcpy(newProduct->restrictions, token);

        size_t len = strlen(newProduct->restrictions);
        if (len > 0 && newProduct->restrictions[len - 1] == '\n') {
            newProduct->restrictions[len - 1] = '\0';
        }

        newProduct->next = *head;
        *head = newProduct;
    }

    fclose(file);
}