/*
* AntiDupl.NET Program.
*
* Copyright (c) 2002-2013 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy 
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
* copies of the Software, and to permit persons to whom the Software is 
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in 
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace AntiDupl.NET
{
    static public class StringsDefaultRussian
    {
        static public void CopyTo(Strings s)
        {
            s.Name = "Russian";
            s.OriginalLanguageName = "Русский";
            
            s.OkButton_Text = "OK";
            s.CancelButton_Text = "Отмена";
            s.StopButton_Text = "Стоп";
            s.SetDefaultButton_Text = "По умолчанию";

            s.ErrorMessage_FileAlreadyExists = "Невозможно переименовать файл, так как файл с таким именем уже существует!";

            s.WarningMessage_ChangeFileExtension = "Вы действительно хотите изменить расширение файла?";

            s.AboutProgramPanel_VersionLabel_Text = "Версия - ";
            s.AboutProgramPanel_RevisionLabel_Text = "Ревизия - ";
            s.AboutProgramPanel_TimeLabel_Text = "2002-2013";
            s.AboutProgramPanel_PlaceLabel_Text = "Минск, Беларусь";
            s.AboutProgramPanel_AuthorLabel_Text = "Ермолаев Игорь";

            s.AboutProgramForm_Text = "О программе...";

            s.StartFinishForm_LoadImages_Text = "Загрузка файла картинок";
            s.StartFinishForm_LoadMistakes_Text = "Загрузка файла ошибок";
            s.StartFinishForm_LoadResults_Text = "Загрузка файла результатов";
            s.StartFinishForm_SaveImages_Text = "Сохранение файла картинок";
            s.StartFinishForm_SaveMistakes_Text = "Сохранение файла ошибок";
            s.StartFinishForm_SaveResults_Text = "Сохранение файла результатов";
            s.StartFinishForm_ClearTemporary_Text = "Удаление временных файлов";

            s.CoreOptionsForm_Text = "Опции";

            s.CoreOptionsForm_SearchTabPage_Text = "Поиск";
            s.CoreOptionsForm_SearchFileTypeGroupBox_Text = "Искать типы:";
            s.CoreOptionsForm_BmpCheckBox_Text = "BMP";
            s.CoreOptionsForm_GifCheckBox_Text = "GIF";
            s.CoreOptionsForm_JpegCheckBox_Text = "JPEG";
            s.CoreOptionsForm_PngCheckBox_Text = "PNG";
            s.CoreOptionsForm_TiffCheckBox_Text = "TIFF";
            s.CoreOptionsForm_EmfCheckBox_Text = "EMF";
            s.CoreOptionsForm_WmfCheckBox_Text = "WMF";
            s.CoreOptionsForm_ExifCheckBox_Text = "EXIF";
            s.CoreOptionsForm_IconCheckBox_Text = "ICON";
            s.CoreOptionsForm_Jp2CheckBox_Text = "JP2";
            s.CoreOptionsForm_PsdCheckBox_Text = "PSD"; 
            s.CoreOptionsForm_SearchSystemCheckBox_Text = "Искать системные каталоги/файлы";
            s.CoreOptionsForm_SearchHiddenCheckBox_Text = "Искать скрытые каталоги/файлы";
            s.CoreOptionsForm_SearchSubFoldersCheckBox_Text = "Искать в подкаталогах";

            s.CoreOptionsForm_CheckTabPage_Text = "Проверка";
            s.CoreOptionsForm_CheckOnDefectCheckBox_Text = "Проверять на дефекты";
            s.CoreOptionsForm_CheckOnEqualityCheckBox_Text = "Искать дублирующие картинки";
            s.CoreOptionsForm_TransformedImageCheckBox_Text = "Искать повернутые и зеркальные дубликаты";
            s.CoreOptionsForm_SizeControlCheckBox_Text = "Учитывать размер картинок";
            s.CoreOptionsForm_TypeControlCheckBox_Text = "Учитывать тип картинок";
            s.CoreOptionsForm_RatioControlCheckBox_Text = "Учитывать соотношение ширины/высоты картинок";
            s.CoreOptionsForm_ThresholdDifferenceLabeledComboBox_Text = "Пороговая разность";
            s.CoreOptionsForm_MinimalImageSizeLabeledIntegerEdit_Text = "Минимальная ширина/высота картинок";
            s.CoreOptionsForm_CompareInsideOneFolderCheckBox_Text = "Сравнивать картинки из одного пути друг с другом";

            s.CoreOptionsForm_AdvancedTabPage_Text = "Дополнительные";
            s.CoreOptionsForm_DeleteToRecycleBinCheckBox_Text = "Удалять в корзину";
            s.CoreOptionsForm_MistakeDataBaseCheckBox_Text = "Запоминать ложные срабатывания";
            s.CoreOptionsForm_RatioResolutionLabeledComboBox_Text = "Точность соотношения ширины/высоты картинок";
            s.CoreOptionsForm_CompareThreadCountLabeledComboBox_Text = "Количество потоков сравнения";
            s.CoreOptionsForm_CompareThreadCountLabeledComboBox_Description_0 = "Авто";
            s.CoreOptionsForm_CollectThreadCountLabeledComboBox_Text = "Количество потоков загрузки";
            s.CoreOptionsForm_CollectThreadCountLabeledComboBox_Description_0 = "Авто";
            s.CoreOptionsForm_ReducedImageSizeLabeledComboBox_Text = "Отнормированный размер картинок";
            s.CoreOptionsForm_UndoQueueSizeLabeledIntegerEdit_Text = "Размер очереди отмены";
            s.CoreOptionsForm_ResultCountMaxLabeledIntegerEdit_Text = "Максимальное количество результатов";
            s.CoreOptionsForm_IgnoreFrameWidthLabeledComboBox_Text = "Ширина игнорируемой рамки картинки";

            s.CorePathsForm_Text = "Пути";
            s.CorePathsForm_SearchTabPage_Text = "Искать";
            s.CorePathsForm_IgnoreTabPage_Text = "Пропускать";
            s.CorePathsForm_ValidTabPage_Text = "Проверено";
            s.CorePathsForm_DeleteTabPage_Text = "Удалять";
            s.CorePathsForm_AddFolderButton_Text = "Добавить каталог";
            s.CorePathsForm_AddFilesButton_Text = "Добавить файлы";
            s.CorePathsForm_ChangeButton_Text = "Изменить";
            s.CorePathsForm_RemoveButton_Text = "Убрать";

            s.ProgressUtils_Completed = "завершено {0}%";
            s.ProgressUtils_5HoursRemaining = "осталось {0} часов";
            s.ProgressUtils_2HoursRemaining = "осталось {0} часа";
            s.ProgressUtils_5MinutesRemaining = "осталось {0} минут";
            s.ProgressUtils_2MinutesRemaining = "осталось {0} минуты";
            s.ProgressUtils_5SecondsRemaining = "осталось {0} секунд";

            s.ProgressForm_DeleteDefect = "Удаление испорченных картинок в выделенных результатах";
            s.ProgressForm_DeleteFirst = "Удаление первых картинок в выделенных результатах";
            s.ProgressForm_DeleteSecond = "Удаление вторых картинок в выделенных результатах";
            s.ProgressForm_DeleteBoth = "Удаление всех парных картинок в выделенных результатах";
            s.ProgressForm_PerformHint = "Автоматическая обработка выделенных результатов";
            s.ProgressForm_Mistake = "Пометка выделенных результатов как ошибочных";
            s.ProgressForm_RenameCurrent = "Переименование/перемещение изображений";
            s.ProgressForm_RefreshResults = "Обновление результатов";
            s.ProgressForm_Undo = "Отмена действия";
            s.ProgressForm_Redo = "Повторение действия";

            s.SearchExecuterForm_Result = "Обработка результатов";
            s.SearchExecuterForm_Search = "Поиск";
            s.SearchExecuterForm_Stopped = "Остановка поиска";
            s.SearchExecuterForm_MinimizeToTaskbarButton_Text = "Свернуть";
            s.SearchExecuterForm_MinimizeToSystrayButton_Text = "Фоновый режим";

            s.ResultsPreviewBase_NextButton_ToolTip_Text = "Перейти к следующему результату";
            s.ResultsPreviewBase_PreviousButton_ToolTip_Text = "Перейти к предыдущему результату";

            s.ResultsPreviewDuplPair_DeleteFirstButton_ToolTip_Text = "Удалить первую картинку";
            s.ResultsPreviewDuplPair_DeleteSecondButton_ToolTip_Text = "Удалить вторую картинку";
            s.ResultsPreviewDuplPair_DeleteBothButton_ToolTip_Text = "Удалить обе картинки";
            s.ResultsPreviewDuplPair_RenameFirstToSecondButton_ToolTip_Text = "Заместить вторую картинку первой";
            s.ResultsPreviewDuplPair_RenameSecondToFirstButton_ToolTip_Text = "Заместить первую картинку второй";
            s.ResultsPreviewDuplPair_MistakeButton_ToolTip_Text = "Пометить текущий результат как ошибочный";

            s.ResultsPreviewDefect_DeleteButton_ToolTip_Text = "Удалить картинку";
            s.ResultsPreviewDefect_MistakeButton_ToolTip_Text = "Пометить текущий результат как ошибочный";

            s.ResultRowSetter_DefectIcon_ToolTip_Text = "Дефектная картинка";
            s.ResultRowSetter_DuplPairIcon_ToolTip_Text = "Дублирующая пара картинок";

            s.ResultRowSetter_UnknownDefectIcon_ToolTip_Text = "Неизвестный дефект";
            s.ResultRowSetter_JpegEndMarkerIsAbsentIcon_ToolTip_Text = "Отсутствует маркер конца JPEG файла";

            s.ResultRowSetter_DeleteDefectIcon_ToolTip_Text = "Удалить картинку";
            s.ResultRowSetter_DeleteFirstIcon_ToolTip_Text = "Удалить первую картинку";
            s.ResultRowSetter_DeleteSecondIcon_ToolTip_Text = "Удалить вторую картинку";
            s.ResultRowSetter_RenameFirstToSecondIcon_ToolTip_Text = "Заместить вторую картинку первой";
            s.ResultRowSetter_RenameSecondToFirstIcon_ToolTip_Text = "Заместить первую картинку второй";

            s.ResultRowSetter_Turn_0_Icon_ToolTip_Text = "Оригинал";
            s.ResultRowSetter_Turn_90_Icon_ToolTip_Text = "Повернутый на 90°";
            s.ResultRowSetter_Turn_180_Icon_ToolTip_Text = "Повернутый на 180°";
            s.ResultRowSetter_Turn_270_Icon_ToolTip_Text = "Повернутый на 270°";
            s.ResultRowSetter_MirrorTurn_0_Icon_ToolTip_Text = "Горизонтально отраженный";
            s.ResultRowSetter_MirrorTurn_90_Icon_ToolTip_Text = "Горизонтально отраженный и повернутый на 90°";
            s.ResultRowSetter_MirrorTurn_180_Icon_ToolTip_Text = "Горизонтально отраженный и повернутый на 180°";
            s.ResultRowSetter_MirrorTurn_270_Icon_ToolTip_Text = "Горизонтально отраженный и повернутый на 270°";

            s.ResultsListView_Type_Column_Text = "Тип";
            s.ResultsListView_Group_Column_Text = "Группа";
            s.ResultsListView_Difference_Column_Text = "Различие";
            s.ResultsListView_Defect_Column_Text = "Тип дефекта";
            s.ResultsListView_Transform_Column_Text = "Трансформация";
            s.ResultsListView_Hint_Column_Text = "Рекомендация";
            
            s.ResultsListView_FileName_Column_Text = "Имя";
            s.ResultsListView_FileDirectory_Column_Text = "В каталоге";
            s.ResultsListView_ImageSize_Column_Text = "Размеры";
            s.ResultsListView_ImageType_Column_Text = "Тип картинки";
            s.ResultsListView_FileSize_Column_Text = "Размер";
            s.ResultsListView_FileTime_Column_Text = "Дата изменения";

            s.ResultsListView_FirstFileName_Column_Text = "1: Имя";
            s.ResultsListView_FirstFileDirectory_Column_Text = "1: В каталоге";
            s.ResultsListView_FirstImageSize_Column_Text = "1: Размеры";
            s.ResultsListView_FirstImageType_Column_Text = "1: Тип картинки";
            s.ResultsListView_FirstFileSize_Column_Text = "1: Размер";
            s.ResultsListView_FirstFileTime_Column_Text = "1: Дата изменения";
            s.ResultsListView_SecondFileName_Column_Text = "2: Имя";
            s.ResultsListView_SecondFileDirectory_Column_Text = "2: В каталоге";
            s.ResultsListView_SecondImageSize_Column_Text = "2: Размеры";
            s.ResultsListView_SecondImageType_Column_Text = "2: Тип картинки";
            s.ResultsListView_SecondFileSize_Column_Text = "2: Размер";
            s.ResultsListView_SecondFileTime_Column_Text = "2: Дата изменения";

            s.ResultsListViewContextMenu_DeleteDefectItem_Text = "Удалить испорченные картинки в выделенных результатах";
            s.ResultsListViewContextMenu_DeleteFirstItem_Text = "Удалить первые картинки в выделенных результатах";
            s.ResultsListViewContextMenu_DeleteSecondItem_Text = "Удалить вторые картинки в выделенных результатах";
            s.ResultsListViewContextMenu_DeleteBothItem_Text = "Удалить все парные картинки в выделенных результатах";
            s.ResultsListViewContextMenu_MistakeItem_Text = "Пометить выделенные результаты как ошибочные";
            s.ResultsListViewContextMenu_PerformHintItem_Text = "Автоматически обработать выделенные результаты";

            s.MainStatusStrip_TotalLabel_Text = "Всего: ";
            s.MainStatusStrip_CurrentLabel_Text = "Текущий: ";
            s.MainStatusStrip_SelectedLabel_Text = "Выделено: ";

            s.MainMenu_FileMenuItem_Text = "Файл";
            s.MainMenu_File_SaveMenuItem_Text = "Сохранить результаты";
            s.MainMenu_File_LoadMenuItem_Text = "Загрузить результаты";
            s.MainMenu_File_ExitMenuItem_Text = "Выход";

            s.MainMenu_EditMenuItem_Text = "Правка";
            s.MainMenu_Edit_UndoMenuItem_Text = "Отменить (Ctrl-Z)";
            s.MainMenu_Edit_RedoMenuItem_Text = "Повторить (Ctrl-Y)";
            s.MainMenu_Edit_SelectAllMenuItem_Text = "Выделить все (Ctrl-A)";

            s.MainMenu_ViewMenuItem_Text = "Вид";
            s.MainMenu_View_ToolMenuItem_Text = "Панель инструментов";
            s.MainMenu_View_StatusMenuItem_Text = "Строка состояния";
            s.MainMenu_View_SelectColumnsMenuItem_Text = "Выбор столбцов";
            s.MainMenu_View_HotKeysMenuItem_Text = "Горячие клавиши";
            s.MainMenu_View_StretchSmallImagesMenuItem_Text = "Растягивать небольшие изображения";
            s.MainMenu_View_ProportionalImageSizeMenuItem_Text = "Пропорциональный размер картинок";

            s.MainMenu_SearchMenuItem_Text = "Поиск";
            s.MainMenu_Search_StartMenuItem_Text = "Начать поиск";
            s.MainMenu_Search_RefreshResultsMenuItem_Text = "Обновить результаты";
            s.MainMenu_Search_RefreshImagesMenuItem_Text = "Обновить базу данных о картинках";
            s.MainMenu_Search_PathsMenuItem_Text = "Пути";
            s.MainMenu_Search_OptionsMenuItem_Text = "Опции";
            s.MainMenu_Search_OnePathMenuItem_Text = "Один путь поиска";
            s.MainMenu_Search_UseImageDataBaseMenuItem_Text = "Использовать базу данных о картинках";
            s.MainMenu_Search_CheckResultsAtLoadingMenuItem_Text = "Проверять результаты при загрузке";

            s.MainMenu_HelpMenuItem_Text = "Справка";
            s.MainMenu_Help_HelpMenuItem_Text = "Справка";
            s.MainMenu_Help_AboutProgramMenuItem_Text = "О программе...";
            s.MainMenu_Help_CheckingForUpdatesMenuItem_Text = "Проверять наличие обновлений";

            s.MainMenu_NewVersionMenuItem_Text = "Новая версия";
            s.MainMenu_NewVersionMenuItem_Tooltip = "AntiDupl.NET-{0} доступна на сайте!";

            s.SelectHotKeysForm_InvalidHotKeyToolTipText = "Эта комбинация клавиш уже используется для других целей.";
            
            s.LanguageMenuItem_Text = "Язык";

            s.ViewModeMenuItem_Text = "Режим просмотра результатов";
            s.ViewModeMenuItem_VerticalPairTableMenuItem_Text = "Таблица вертикальных пар";
            s.ViewModeMenuItem_HorizontalPairTableMenuItem_Text = "Таблица горизонтальных пар";
            s.ViewModeMenuItem_GroupedThumbnailsMenuItem_Text = "Сгруппированные миниатюры";

            s.ImagePreviewContextMenu_CopyPathItem_Text = "Скопировать путь";
            s.ImagePreviewContextMenu_OpenImageItem_Text = "Открыть картинку";
            s.ImagePreviewContextMenu_OpenFolderItem_Text = "Открыть каталог с картинкой";
            s.ImagePreviewContextMenu_RenameImageItem_Text = "Переименовать картинку";
            s.ImagePreviewContextMenu_MoveImageToNeighbourItem_Text = "Переместить картинку к соседней";
        }

        static private Strings Create()
        {
            Strings strings = new Strings();
            CopyTo(strings);
            return strings;
        }

        static public Strings Get()
        {
            return m_strings;
        }

        static private Strings m_strings = Create();
    }
}
