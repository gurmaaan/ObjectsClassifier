# Описание формата файла

## Обозначения

>&quot;id&quot;.&quot;x&quot;.&quot;y&quot;.&quot;x&quot;.&quot;y&quot;.&quot;x&quot;.&quot;y&quot;.&quot;x&quot;.&quot;y&quot;...&quot;…&quot;.&quot;C&quot;. &quot;x&quot;.&quot;y&quot;…&quot;...&quot;. &quot;M&quot;.&quot;AvBrR&quot;.&quot;AvBrG&quot;.&quot;AvBrB&quot;.&quot;SKOR&quot;.
&quot;SKOG&quot;.&quot;SKOB&quot;.&quot;IntervalR&quot;.&quot;IntervalG&quot;.&quot;IntervalB&quot;.&quot;TextureR&quot;.&quot;TextureG&quot;.&quot;TextureB&quot;.&quot;LenthR&quot;.
&quot;LenthG&quot;.&quot;LenthB&quot;.&quot;Form&quot;.&quot;Area&quot;.&quot;Perimetr&quot;;


## Метасимволы

Разделитель – **&quot;.&quot;** , конец записи – **&quot;;&quot;**.

## Значения кодов

**ID** – целое число, обозначает идентификатор (номер) объекта. Служит началом записи. После этого числа идет **перечисление точек** , содержащих сам объект. Координаты через точку X.Y.

**С** – код, обозначающий начало **перечисления координат контура**.

**М** – код, обозначающий начало **перечисления рассчитанных параметров** указанного объекта.

## Порядок **параметров**

1. **AvBrR** - Средняя яркость R;
2. **AvBrG** - Средняя яркость G;
3. **AvBrB** - Средняя яркость B;
4. **SKOR** - СКО R;
5. **SKOG** - СКО G;
6. **SKOB** - СКО B;
7. **Interval**** R** - Диапазон R;
8. **IntervalG** - Диапазон G;
9. **IntervalB** - Диапазон B;
10. **TextureR** - Текстур признак МПС R;
11. **TextureG** - Текстур признак МПС G;
12. **TextureB** - Текстур признак МПС B;
13. **LenthR** - Тек признак длины серий R;
14. **LenthG** - Тек признак длины серий G;
15. **LenthB** - Тек признак длины серий B;
16. **Form** – Форма;
17. **Area** – Площадь;
18. **Perimetr**– Периметр;

## Пример

![Test Image][logo]

[logo]: https://pp.userapi.com/c840439/v840439790/30d1b/84E6-Jtnbys.jpg "Image for format description"

0.2.1.1.2.2.2.2.0.0.2.C.2.1.1.2.2.2.2.0.0.2.M...;
1.3.4.4.3.4.4.C.3.4.4.3.4.4.M...;