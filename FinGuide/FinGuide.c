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
        printf("메뉴를 선택하세요: ");
        scanf("%d", &choice);
        HMain(choice);
    }

    return 0;
}

void mainMenu() {
    printf("\n===== 메인 메뉴 =====\n");
    printf("1. 예금 상품 추천\n");
    printf("2. 적금 상품 추천\n");
    printf("3. 예금 상품 추가\n");
    printf("4. 적금 상품 추가\n");
    printf("5. 예금 상품 삭제\n");
    printf("6. 적금 상품 삭제\n");
    printf("7. 상품 검색\n");
    printf("8. 프로그램 종료\n");
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
        printf("저축 금액을 입력하세요: ");
        scanf("%lf", &amount);
        printf("저축 기간을 입력하세요(개월): ");
        scanf("%d", &period);
        printf("주 이용 은행을 입력하세요: ");
        getchar();
        fgets(bank, sizeof(bank), stdin);
        bank[strcspn(bank, "\n")] = '\0';
        printf("나이를 입력하세요: ");
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
        printf("\n===== 상품 검색 =====\n");
        printf("1. 예금 상품 검색\n");
        printf("2. 적금 상품 검색\n");
        printf("=====================\n");
        printf("검색할 카테고리를 선택하세요: ");
        scanf("%d", &choice);
        if (choice == 1) {
            SearchProduct(depositHead);
        }
        else if (choice == 2) {
            SearchProduct(savingHead);
        }
        else {
            printf("잘못된 선택입니다. 다시 선택해주세요.\n");
        }
        break;
    case 8:
        printf("프로그램을 종료합니다.\n");
        SaveProducts(depositHead, "depositProducts.txt");
        SaveProducts(savingHead, "savingProducts.txt");
        exit(0);
    default:
        printf("잘못된 선택입니다. 다시 선택해주세요.\n");
    }
}

void AddProduct(Product** head) {
    Product* newProduct = (Product*)malloc(sizeof(Product));
    if (newProduct == NULL) {
        printf("메모리 할당에 실패했습니다.\n");
        return;
    }

    printf("상품 이름을 입력하세요: ");
    getchar();
    fgets(newProduct->name, sizeof(newProduct->name), stdin);
    newProduct->name[strcspn(newProduct->name, "\n")] = '\0';
    printf("은행 이름을 입력하세요: ");
    fgets(newProduct->bank, sizeof(newProduct->bank), stdin);
    newProduct->bank[strcspn(newProduct->bank, "\n")] = '\0';
    printf("세후 이자율을 입력하세요: ");
    scanf("%lf", &newProduct->afterTaxInterestRate);
    printf("기간(개월)을 입력하세요: ");
    scanf("%d", &newProduct->period);
    printf("제한 사항(예: 나이<30 또는 20<나이<30)을 입력하세요: ");
    getchar();
    fgets(newProduct->restrictions, sizeof(newProduct->restrictions), stdin);
    newProduct->restrictions[strcspn(newProduct->restrictions, "\n")] = '\0';

    newProduct->next = *head;
    *head = newProduct;
    printf("상품이 추가되었습니다.\n");
}

void RemoveProduct(Product** head) {
    if (*head == NULL) {
        printf("삭제할 상품이 없습니다.\n");
        return;
    }

    char productName[100];
    int productPeriod;
    printf("삭제할 상품 이름을 입력하세요: ");
    getchar();
    fgets(productName, sizeof(productName), stdin);
    productName[strcspn(productName, "\n")] = '\0';
    printf("삭제할 상품의 기간(개월)을 입력하세요: ");
    scanf("%d", &productPeriod);

    Product* current = *head;
    Product* prev = NULL;

    while (current != NULL && (strcmp(current->name, productName) != 0 || current->period != productPeriod)) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("해당 이름과 기간의 상품을 찾을 수 없습니다.\n");
        return;
    }

    if (prev == NULL) {
        *head = current->next;
    }
    else {
        prev->next = current->next;
    }

    free(current);
    printf("상품이 삭제되었습니다.\n");
}

void SearchProduct(Product* head) {
    if (head == NULL) {
        printf("검색할 상품이 없습니다.\n");
        return;
    }

    char productName[100];
    char bankName[100];
    int productPeriod;
    printf("검색할 상품 이름을 입력하세요: ");
    getchar();
    fgets(productName, sizeof(productName), stdin);
    productName[strcspn(productName, "\n")] = '\0';
    printf("검색할 은행 이름을 입력하세요: ");
    fgets(bankName, sizeof(bankName), stdin);
    bankName[strcspn(bankName, "\n")] = '\0';
    printf("검색할 상품의 기간(개월)을 입력하세요: ");
    scanf("%d", &productPeriod);

    Product* current = head;
    while (current != NULL) {
        if (strcmp(current->name, productName) == 0 && strcmp(current->bank, bankName) == 0 && current->period == productPeriod) {
            printf("\n상품 정보를 찾았습니다:\n");
            printf("상품 이름: %s\n", current->name);
            printf("은행: %s\n", current->bank);
            printf("세후 이자율: %.2f%%\n", current->afterTaxInterestRate);
            printf("기간: %d개월\n", current->period);
            printf("제한 사항: %s\n", current->restrictions);
            return;
        }
        current = current->next;
    }

    printf("해당 조건에 맞는 상품을 찾을 수 없습니다.\n");
}

void RecommendProduct(Product* head, double amount, int period, const char* bank, int age) {
    if (head == NULL) {
        printf("추천할 상품이 없습니다.\n");
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
        printf("\n전체 상품 중 추천 상품:\n");
        printf("상품 이름: %s\n", bestProduct->name);
        printf("은행: %s\n", bestProduct->bank);
        printf("세후 이자율: %.2f%%\n", bestProduct->afterTaxInterestRate);
        printf("기간: %d개월\n", bestProduct->period);
        printf("제한 사항: %s\n", bestProduct->restrictions);
        printf("세후 수령액: %d원\n", (int)bestProductReturn);
    }
    else {
        printf("전체 상품 중 조건에 맞는 추천 상품이 없습니다.\n");
    }

    if (bestBankProduct != NULL) {
        printf("\n주 이용 은행 중 추천 상품:\n");
        printf("상품 이름: %s\n", bestBankProduct->name);
        printf("은행: %s\n", bestBankProduct->bank);
        printf("세후 이자율: %.2f%%\n", bestBankProduct->afterTaxInterestRate);
        printf("기간: %d개월\n", bestBankProduct->period);
        printf("제한 사항: %s\n", bestBankProduct->restrictions);
        printf("세후 수령액: %d원\n", (int)bestBankProductReturn);
    }
    else {
        printf("주 이용 은행 중 조건에 맞는 추천 상품이 없습니다.\n");
    }
}

int CheckRestrictions(const char* restrictions, double amount, int period, const char* bank, int age) {
    int minAge = 0, maxAge = 0;

    if (strstr(restrictions, "나이<") != NULL && sscanf(restrictions, "나이<%d", &maxAge) == 1) {
        if (age >= maxAge) return 0;
    }
    else if (strstr(restrictions, "나이>") != NULL && sscanf(restrictions, "나이>%d", &minAge) == 1) {
        if (age <= minAge) return 0;
    }
    else if (strstr(restrictions, "나이=") != NULL && sscanf(restrictions, "나이=%d", &minAge) == 1) {
        if (age != minAge) return 0;
    }
    else if (strstr(restrictions, "나이<=") != NULL && sscanf(restrictions, "나이<=%d", &maxAge) == 1) {
        if (age > maxAge) return 0;
    }
    else if (strstr(restrictions, "나이>=") != NULL && sscanf(restrictions, "나이>=%d", &minAge) == 1) {
        if (age < minAge) return 0;
    }
    else if (sscanf(restrictions, "%d<=나이<=%d", &minAge, &maxAge) == 2) {
        if (age < minAge || age > maxAge) return 0;
    }

    return 1;
}

void SaveProducts(Product* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("파일을 저장할 수 없습니다: %s\n", filename);
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
        printf("파일을 열 수 없습니다: %s\n", filename);
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        Product* newProduct = (Product*)malloc(sizeof(Product));
        if (newProduct == NULL) {
            printf("메모리 할당에 실패했습니다.\n");
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