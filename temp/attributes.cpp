enum class Attribute 
{
    //Яркостные признаки 0-2. Средняя яркость по компонентам
    //Вычисляется как суммарная яркость / кол-во пикселей
	BrRAv,
	BrGAv,
	BrRAv,

	//Яркостные признаки 3-5. СКО яркости по компонентам.
	//sqrt { sum[ (ср я - я i)^2 / N ] }
	BrRsko,
	BrGsko,
	BrBsko,

	//Текстурные признаки 6-8. МПС  по компонентам - энергия. 
    //По факту - среднее значение квадрата яркости компонента
	TextRmps
	TextGmps,
	TextBmps,

    //Текстурные признаки 9-11. Длина серии по компонентам - энтропия. 
    //По факту - среднее значение величины R * ln (R) 
	TextRser,
	TextGser,
	TextBser,

    //Геометрические признаки. 
    // S = площадь, P = периметр, K = P^2 / S
	GeomForm,
	GeomSquare,
	GeomPerim
};


// prepare x axis with country labels:
QVector<double> ticks;
QVector<QString> labels;

ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7;
labels << "USA" << "Japan" << "Germany" << "France" << "UK" << "Italy" << "Canada";

QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
textTicker->addTicks(ticks, labels);

customPlot->xAxis->setTicker(textTicker);
customPlot->xAxis->setTickLabelRotation(60);
customPlot->xAxis->setSubTicks(false);
customPlot->xAxis->setTickLength(0, 4);

customPlot->xAxis->setRange(0, 8);

customPlot->xAxis->setBasePen(QPen(Qt::white));
customPlot->xAxis->setTickPen(QPen(Qt::white));

customPlot->xAxis->grid()->setVisible(true);
customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

customPlot->xAxis->setTickLabelColor(Qt::white);
customPlot->xAxis->setLabelColor(Qt::white);

// prepare y axis:
customPlot->yAxis->setRange(0, 12.1);
customPlot->yAxis->setPadding(5); // a bit more space to the left border
customPlot->yAxis->setLabel("Power Consumption in\nKilowatts per Capita (2007)");
customPlot->yAxis->setBasePen(QPen(Qt::white));
customPlot->yAxis->setTickPen(QPen(Qt::white));
customPlot->yAxis->setSubTickPen(QPen(Qt::white));
customPlot->yAxis->grid()->setSubGridVisible(true);
customPlot->yAxis->setTickLabelColor(Qt::white);
customPlot->yAxis->setLabelColor(Qt::white);
customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

// Add data:
QVector<double> fossilData, nuclearData, barData;
fossilData  << 0.86*10.5 << 0.83*5.5 << 0.84*5.5 << 0.52*5.8 << 0.89*5.2 << 0.90*4.2 << 0.67*11.2;
nuclearData << 0.08*10.5 << 0.12*5.5 << 0.12*5.5 << 0.40*5.8 << 0.09*5.2 << 0.00*4.2 << 0.07*11.2;
barData   << 0.06*10.5 << 0.05*5.5 << 0.04*5.5 << 0.06*5.8 << 0.02*5.2 << 0.07*4.2 << 0.25*11.2;
fossil->setData(ticks, fossilData);
nuclear->setData(ticks, nuclearData);
bar->setData(ticks, barData);

// setup legend:
customPlot->legend->setVisible(true);
customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
customPlot->legend->setBrush(QColor(255, 255, 255, 100));
customPlot->legend->setBorderPen(Qt::NoPen);
QFont legendFont = font();
legendFont.setPointSize(10);
customPlot->legend->setFont(legendFont);
customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);