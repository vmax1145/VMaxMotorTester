Chart.defaults.font.family = '"Verdana", sans-serif';
Chart.defaults.font.size = 14;
Chart.defaults.font.weight = 'bold';
Chart.defaults.color = 'black';
Chart.defaults.elements.point.radius = 3;
var chart = new Chart(document.getElementById("line-chart"), {
    type: 'scatter',

    data: {
        datasets: [
            {
                label:"Thrust (g)",
                cubicInterpolationMode: 'monotone',
                showLine: true,
                yAxisID: 'yTr',
                borderColor: '#006000',
                data: []
            },
            {
                label:"RPM",
                cubicInterpolationMode: 'monotone',
                showLine: true,
                yAxisID: 'yRpm',
                borderColor: '#0000a0',
                data: []
            },
            {
                label:"Сurrent (A)",
                cubicInterpolationMode: 'monotone',
                showLine: true,
                yAxisID: 'yCur',
                borderColor: '#b00000',
                data: []
            },
            {
                label:"Voltage (V)",
                cubicInterpolationMode: 'monotone',
                showLine: true,
                yAxisID: 'yVolt',
                borderColor: '#606060',
                data: []
            },
            {
                label:"Power (W)",
                cubicInterpolationMode: 'monotone',
                showLine: true,
                yAxisID: 'yPow',
                borderColor: '#b0a000',
                data: []
            }
        ]
    },

    options: {
        scales: {
            x: {type: 'linear', min: 0, max:100, title: {display: true,text: 'Throttle %'}},
            yTr: {type: 'linear', min: 0, position: 'right', ticks: {color:'#006000'}, title: {display: true,color:'#006000' }}, //text: 'Thrust (gr)',
            yRpm: {type: 'linear', min: 0, position: 'right',ticks: {source: 'labels', color:'#0000a0'}, title: {display: true, color:'#0000a0'}}, //text: 'RPM',
            yCur: {type: 'linear', min: 0, ticks: {color:'#b00000'}, title: {display: true, color:'#b00000'}}, //text:'Current (A)',
            yVolt: {type: 'linear', min: 0, ticks: {color:'#606060'}, title: {display: true, color:'#606060'}}, //,text: 'Voltage (V)'
            yPow: {type: 'linear', min: 0, ticks: {color:'#b0a000'}, title: {display: true, color:'#b0a000'}} //text: 'Power (W)',
        },
        parsing: false,
        animation: false,
        plugins: {
            legend: {


                onClick: (e, legendItem, legend) => {
                    const index = legendItem.datasetIndex;
                    const chart = legend.chart;
                    const meta = chart.getDatasetMeta(index);

                    // Toggle dataset visibility
                    chart.setDatasetVisibility(index, !chart.isDatasetVisible(index));

                    // Find the associated axis and toggle its visibility
                    const axisId = meta.yAxisID || meta.xAxisID;
                    if (axisId) {
                        // Check if ANY other visible dataset is still using this axis
                        const anyVisible = chart.data.datasets.some((ds, i) =>
                            chart.isDatasetVisible(i) && (chart.getDatasetMeta(i).yAxisID === axisId || chart.getDatasetMeta(i).xAxisID === axisId)
                        );
                        chart.options.scales[axisId].display = anyVisible;
                    }

                    chart.update();
                }

            }
        }
    }
});

function clearCharts() {
    chart.data.datasets.forEach((dataset) => {
        dataset.data = []; // Clear data for each dataset
    });
    chart.update();
}

var datasetsVisible = [true,true,true,true,true];
function toggleChart(inx) {
    datasetsVisible[inx] = !datasetsVisible[inx];
    chart.setDatasetVisibility(inx,datasetsVisible[inx]);
}

function updateChart(throttle, lastMeasure) {
    chart.data.datasets[0].data.push({x:throttle,y:lastMeasure["thrust"]});
    chart.data.datasets[1].data.push({x:throttle,y:lastMeasure["rpm"]});
    chart.data.datasets[2].data.push({x:throttle,y:lastMeasure["current"]});
    chart.data.datasets[3].data.push({x:throttle,y:lastMeasure["voltage"]});
    chart.data.datasets[4].data.push({x:throttle,y:(lastMeasure["voltage"]*lastMeasure["current"])});
    chart.update();
}

function getCsv() {

    var ret = 'throttle, thrust, rpm, current, voltage\n'
    chart.data.datasets[0].data.forEach( (r,inx)=> {
        ret += r.x+','+r.y +','
            + chart.data.datasets[1].data[inx].y +','
            + chart.data.datasets[2].data[inx].y +','
            + chart.data.datasets[3].data[inx].y
            +'\n'
        ;
    });
    return ret;
}
function asFloat(v) {
    v = v.replace('\s','','gi')
    return Number.parseFloat(v);
}
function loadChartData(value) {
    chart.data.datasets.forEach((dataset) => {
        dataset.data = []; // Clear data for each dataset
    });
    chart.update();
    var rows = value.split('\n')
    rows.forEach((r,inx)=> {

         if(inx>0) {
             var fields = r.split(',');
             if(fields.length === 5) {
                 var throttle = asFloat(fields[0]);
                 chart.data.datasets[0].data.push({x: throttle, y: asFloat(fields[1])});
                 chart.data.datasets[1].data.push({x: throttle, y: asFloat(fields[2])});
                 chart.data.datasets[2].data.push({x: throttle, y: asFloat(fields[3])});
                 chart.data.datasets[3].data.push({x: throttle, y: asFloat(fields[4])});
                 chart.data.datasets[4].data.push({x: throttle, y: asFloat(fields[3])*asFloat(fields[4])});
             }
         }
    });
    chart.update();
}