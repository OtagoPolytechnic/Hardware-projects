from rest_framework import status
from rest_framework.decorators import api_view, authentication_classes, permission_classes
from rest_framework.response import Response
from .models import Sensor, History
from .serializers import SensorSerializer, UserSerializer, HistorySerializer
from django.core.exceptions import ObjectDoesNotExist
from django.contrib.auth.models import User
from rest_framework import generics
from rest_framework.authentication import TokenAuthentication
from rest_framework.permissions import IsAuthenticatedOrReadOnly, IsAdminUser


# Create your views here.

@api_view(['GET', 'POST'])
@authentication_classes((TokenAuthentication,))
@permission_classes((IsAuthenticatedOrReadOnly,))
def sensor_list(request):

    if request.method == 'GET':
        sensors = Sensor.objects.all()
        serializer = SensorSerializer(sensors, many=True)


        return Response(serializer.data, status=status.HTTP_200_OK)

    elif request.method == 'POST':
        serializer = SensorSerializer(data=request.data)
        if serializer.is_valid():
            serializer.save()

            return Response(serializer.data, status=status.HTTP_201_CREATED)

        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)


@api_view(['GET'])
@authentication_classes((TokenAuthentication,))
@permission_classes((IsAuthenticatedOrReadOnly,))
def history_list(request):

    if request.method == 'GET':
        history = History.objects.all()
        serializer = HistorySerializer(history, many=True)

        return Response(serializer.data)


@api_view(['GET', 'PUT', 'DELETE'])
@authentication_classes((TokenAuthentication,))
@permission_classes((IsAuthenticatedOrReadOnly,))
def sensor_details(request, pk):

    if request.method == 'GET':
        try:
            sensor = Sensor.objects.get(pk=pk)
            history = History.objects.filter(sensor=sensor)
        except Sensor.DoesNotExist:
            sensor = Sensor.objects.all()

        sensor_serializer = SensorSerializer(sensor)
        history_serializer = HistorySerializer(history, many=True)

        return Response({
            "sensor": sensor_serializer.data,
            "history": history_serializer.data
        }, status=status.HTTP_200_OK)

    elif request.method == 'DELETE':
        try:
            sensor = Sensor.objects.get(pk=pk)
            sensor.delete()

            return Response(status=status.HTTP_200_OK)
        except ObjectDoesNotExist:
            return Response(status=status.HTTP_400_BAD_REQUEST)


class UserList(generics.ListAPIView):
    queryset = User.objects.all()
    serializer_class = UserSerializer
    authentication_classes = (TokenAuthentication,)
    permission_classes = (IsAdminUser,)


class UserDetail(generics.RetrieveAPIView):
    queryset = User.objects.all()
    serializer_class = UserSerializer
    authentication_classes = (TokenAuthentication,)
    permission_classes = (IsAdminUser,)
