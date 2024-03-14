# ИДЗ 1
<b>Выполнила:</b> Панфилова Мария \
<b>Группа:</b> БПИ226 \
<b>Вриант:</b> 21 \
<b>Желаемая оценка:</b> 8 \
Все программы на соответствующую оценку лежат в соответствующих папках.
## Текст варианта
Разработать программу, вычисляющую число вхождений различных прописных букв в заданной ASCII–строке. В выходном тексте для каждой прописной буквы вывести, сколько раз она
встретилась в обработанном тексте.

## Входные данные
В программах на 4-7 баллов на вход через аргументы командной строки подается сначала название файла с исходными данными, потом название файла для записи результата.
В решении на 8 названия файлов надо передать только программе parent.cpp, child.cpp запускается без пареметров.

## Тесты 
Все примеры тестов и результатов их выполнения лежат [тут](https://github.com/MShpiz/OS_IHW1/tree/main/tests).

## Общее решение на 4-5
Поток родитель читает файл, создаёт 1 пайп и закидывает его в первый пайп, делает 1 ребёнка. 1 ребёнок обрабатывает данные из 1 пайпа, создаёт 2 пайп, кладет результат во 2 пайп и делает второго ребёнка. 2ребёнок читает результат обработки из 2 пайпа и записывает его в файл.

## Общее решение на 6-7
Поток родитель читает файл, создаёт 1 и 2 пайп и закидывает его в первый пайп, делает ребёнка. Ребёнок обрабатывает данные из 1 пайпа, кладет результат во 2 пайп. Родитель читает результат обработки из 2 пайпа и записывает его в файл.

## Общее решение на 8
Программа parent читает файл, создаёт 1 и 2 пайп и закидывает его в первый пайп, делает ребёнка. Программа child обрабатывает данные из 1 пайпа, кладет результат во 2 пайп. Программа parent читает результат обработки из 2 пайпа и записывает его в файл.

## Имена пайпов из программ на 5, 7, 8 и способ их создания
первый пайп: "text.fifo" \
второй пайп: "text2.fifo" \
Создаются как файлы, с модификатором 666.
